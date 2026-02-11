/*
  sam.c - PIC16F877A bare-metal MQ6 (integer-only) + 16x2 LCD (8-bit)
  - ADC: AN0 (RA0)
  - LCD data: RD0..RD7 (bit-level)
  - LCD RS -> RB3, EN -> RB1
  - No floats, no log/pow, small lookup table for ppm mapping
*/

#include <xc.h>
#include <stdint.h>

/* CONFIG */
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000UL

/* LCD pin masks */
#define LCD_RS_MASK 0x08  // RB3
#define LCD_EN_MASK 0x02  // RB1

/* LCD DDRAM addresses */
#define FIRST_LINE  0x80
#define SECOND_LINE 0xC0

/* Hardware constants */
#define RL_OHMS    10000UL     // 10 kOhm in ohms
// represent clean air factor 9.83 as numerator/denominator = 983 / 100
#define RO_NUM     983U
#define RO_DEN     100U

/* Lookup table (precomputed from MQ6 curve) */
/* Pairs: (ratio_scaled, ppm)
   ratio_scaled = (rs/ro) * 1000 (scaled by 1000)
   Table computed from MQ6 curve points for these ppm values:
   ppm: 50, 100, 200, 500, 1000, 2000, 5000, 10000
   ratio values (rounded *1000): 3519,2649,1993,1369,1030,775,533,401
*/
static const uint16_t ratio_table[] = {3519, 2649, 1993, 1369, 1030, 775, 533, 401};
static const uint32_t ppm_table[]   = {50, 100, 200, 500, 1000, 2000, 5000, 10000};
#define TABLE_SIZE (sizeof(ratio_table)/sizeof(ratio_table[0]))

/* Prototypes */
void system_init(void);
void small_delay_ms(unsigned int ms);
void ADC_init(void);
uint16_t ADC_read_raw(void);

void write_data_bits(uint8_t val);
void lcd_enable_pulse(void);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t d);
void LCD_print(const char *s);
void lcd_init(void);
void lcd_clear_line(uint8_t line);
void lcd_print_uint(uint32_t v);

uint32_t calc_Rs_ohm(uint16_t adc_raw);
uint32_t calibrate_Ro(void);
uint32_t read_Rs_avg(void);
uint32_t ratio_scaled_from_RsRo(uint32_t Rs_ohm, uint32_t Ro_ohm);
uint32_t ratio_to_ppm(uint32_t ratio_scaled);

/* Gas detect pin if used */
#define GAS_DETECT_PIN PORTDbits.RD5
#define GAS_DETECT_TRIS TRISDbits.TRISD5

/* ===== main ===== */
void main(void)
{
    system_init();

    // Simple display while calibrating
    lcd_command(FIRST_LINE);
    LCD_print("Calibrating...");
    uint32_t Ro = calibrate_Ro(); // Ro in ohms (integer)
    // show Ro in kilo-ohms with two decimal places scaling
    lcd_command(0x01); // clear
    small_delay_ms(10);
    lcd_command(FIRST_LINE);
    LCD_print("Ro=");
    // print Ro/1000 with two decimals: e.g., 9800 => 9.80 KOhm
    uint32_t Ro_k_whole = Ro / 1000;
    uint32_t Ro_k_frac = (Ro % 1000) / 10; // two digits
    lcd_print_uint(Ro_k_whole);
    lcd_data('.');
    if (Ro_k_frac < 10) lcd_data('0');
    lcd_print_uint(Ro_k_frac);
    LCD_print(" KOhm");
    small_delay_ms(1500);
    lcd_command(0x01);

    while (1)
    {
        // Optional digital gas detect active-low
        if (GAS_DETECT_PIN == 0) {
            lcd_command(FIRST_LINE);
            LCD_print("Gas present     ");
            lcd_command(SECOND_LINE);
            LCD_print("PPM=");
            uint32_t Rs = read_Rs_avg(); // ohms
            uint32_t ratio_s = ratio_scaled_from_RsRo(Rs, Ro); // scaled by 1000
            uint32_t ppm = ratio_to_ppm(ratio_s);
            lcd_print_uint(ppm);
            small_delay_ms(1500);
            lcd_command(0x01);
        } else {
            lcd_command(FIRST_LINE);
            LCD_print("Gas not present ");
            small_delay_ms(500);
        }
    }
}

/* ===== system and init ===== */
void system_init(void)
{
    TRISB = 0x00; // control pins output
    TRISD = 0x00; // data initially outputs (we will set specific bits if needed)
    // set RD5 as input for gas detect if used
    TRISDbits.TRISD5 = 1;
    PORTB = 0x00;
    PORTD = 0x00;
    lcd_init();
    ADC_init();
}

void small_delay_ms(unsigned int ms)
{
    while (ms--) __delay_ms(1);
}

/* ===== ADC ===== */
void ADC_init(void)
{
    ADCON1 = 0x8E; // AN0 analog, others digital, right-justified
    ADCON0 = 0x01; // ADC on, channel 0 selected
    small_delay_ms(2);
}

uint16_t ADC_read_raw(void)
{
    // Select AN0 channel (CHS bits = 000)
    ADCON0 &= 0xC7; // clear CHS bits
    ADCON0 |= (0 << 3);
    small_delay_ms(2); // acquisition
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE);
    uint16_t raw = ((uint16_t)ADRESH << 8) | ADRESL;
    return raw & 0x03FF;
}

/* ===== MQ math (integer) ===== */
/* Rs = RL * (1023 - ADC) / ADC  (RL in ohms) */
uint32_t calc_Rs_ohm(uint16_t adc_raw)
{
    if (adc_raw == 0) return 0xFFFFFFFFUL; // very large (error), avoid div0
    uint32_t numer = (uint32_t)RL_OHMS * (uint32_t)(1023 - adc_raw);
    uint32_t rs = numer / (uint32_t)adc_raw; // ohms
    return rs;
}

/* Calibrate Ro: average N Rs readings then Ro = Rs_avg / 9.83 => use factor 983/100 */
uint32_t calibrate_Ro(void)
{
    const uint8_t samples = 30; // fewer samples to save time and code
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; ++i) {
        uint16_t raw = ADC_read_raw();
        uint32_t rs = calc_Rs_ohm(raw);
        sum += rs;
        small_delay_ms(80);
    }
    uint32_t avg_rs = (uint32_t)(sum / samples); // ohms
    // Ro = avg_rs / 9.83  -> use integer: Ro = (avg_rs * 100) / 983
    uint32_t Ro = (uint32_t)((avg_rs * (uint32_t)RO_DEN) / (uint32_t)RO_NUM);
    return Ro;
}

/* Read Rs average (ohms) over a few samples */
uint32_t read_Rs_avg(void)
{
    const uint8_t samples = 5;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; ++i) {
        uint16_t raw = ADC_read_raw();
        uint32_t rs = calc_Rs_ohm(raw);
        sum += rs;
        small_delay_ms(40);
    }
    return (uint32_t)(sum / samples);
}

/* compute ratio_scaled = (Rs / Ro) * 1000  using integers
   ratio_scaled = (Rs * 1000) / Ro
   if Ro==0 handle */
uint32_t ratio_scaled_from_RsRo(uint32_t Rs_ohm, uint32_t Ro_ohm)
{
    if (Ro_ohm == 0) return 0xFFFFFFFFUL;
    uint32_t numer = (uint32_t)Rs_ohm * 1000ULL;
    uint32_t ratio_s = (uint32_t)(numer / Ro_ohm);
    return ratio_s; // scaled by 1000
}

/* Map ratio_scaled to ppm with simple linear interpolation on the lookup table */
uint32_t ratio_to_ppm(uint32_t ratio_scaled)
{
    // If ratio >= top of table (higher ratio => lower ppm), return lowest ppm (50)
    if (ratio_scaled >= ratio_table[0]) return ppm_table[0];
    // If ratio <= bottom of table, return highest ppm
    if (ratio_scaled <= ratio_table[TABLE_SIZE-1]) return ppm_table[TABLE_SIZE-1];

    // Find interval where ratio_table[i] >= ratio_scaled >= ratio_table[i+1]
    for (uint8_t i = 0; i < (TABLE_SIZE - 1); ++i) {
        uint16_t r1 = ratio_table[i];
        uint16_t r2 = ratio_table[i+1];
        if ((ratio_scaled <= r1) && (ratio_scaled >= r2)) {
            uint32_t p1 = ppm_table[i];
            uint32_t p2 = ppm_table[i+1];
            // Linear interpolation: ppm = p1 + (p2-p1)*( (r1 - ratio) / (r1 - r2) )
            uint32_t r_diff = (uint32_t)(r1 - r2);
            uint32_t r_pos = (uint32_t)(r1 - ratio_scaled); // distance from r1
            // avoid division by zero
            if (r_diff == 0) return p1;
            uint32_t ppm = p1 + ( (p2 - p1) * r_pos ) / r_diff;
            return ppm;
        }
    }
    // fallback
    return 0;
}

/* ===== LCD (bit-level RD0..RD7, RS=RB3, EN=RB1) ===== */
void write_data_bits(uint8_t val)
{
    PORTDbits.RD0 = (val & 0x01) ? 1 : 0;
    PORTDbits.RD1 = (val & 0x02) ? 1 : 0;
    PORTDbits.RD2 = (val & 0x04) ? 1 : 0;
    PORTDbits.RD3 = (val & 0x08) ? 1 : 0;
    PORTDbits.RD4 = (val & 0x10) ? 1 : 0;
    PORTDbits.RD5 = (val & 0x20) ? 1 : 0;
    PORTDbits.RD6 = (val & 0x40) ? 1 : 0;
    PORTDbits.RD7 = (val & 0x80) ? 1 : 0;
}

void lcd_enable_pulse(void)
{
    PORTB |= LCD_EN_MASK;
    small_delay_ms(1);
    PORTB &= ~LCD_EN_MASK;
    small_delay_ms(1);
}

void lcd_command(uint8_t cmd)
{
    // ensure RD pins are outputs
    TRISDbits.TRISD0 = 0; TRISDbits.TRISD1 = 0; TRISDbits.TRISD2 = 0; TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0; TRISDbits.TRISD5 = 0; TRISDbits.TRISD6 = 0; TRISDbits.TRISD7 = 0;
    // RS = 0
    PORTB &= ~LCD_RS_MASK;
    write_data_bits(cmd);
    lcd_enable_pulse();
    small_delay_ms(2);
}

void lcd_data(uint8_t d)
{
    TRISDbits.TRISD0 = 0; TRISDbits.TRISD1 = 0; TRISDbits.TRISD2 = 0; TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0; TRISDbits.TRISD5 = 0; TRISDbits.TRISD6 = 0; TRISDbits.TRISD7 = 0;
    // RS = 1
    PORTB |= LCD_RS_MASK;
    write_data_bits(d);
    lcd_enable_pulse();
    small_delay_ms(1);
}

void LCD_print(const char *s)
{
    while (*s) {
        lcd_data((uint8_t)*s++);
    }
}

void lcd_init(void)
{
    TRISBbits.TRISB3 = 0; // RS output
    TRISBbits.TRISB1 = 0; // EN output
    TRISD = 0x00;         // RD0..RD7 outputs
    PORTB &= ~(LCD_RS_MASK | LCD_EN_MASK);
    PORTD = 0x00;
    small_delay_ms(20);
    // init sequence
    lcd_command(0x30);
    small_delay_ms(5);
    lcd_command(0x30);
    small_delay_ms(5);
    lcd_command(0x30);
    small_delay_ms(5);
    lcd_command(0x38); // 8-bit, 2-line
    small_delay_ms(5);
    lcd_command(0x0C); // display on, cursor off
    small_delay_ms(5);
    lcd_command(0x01); // clear
    small_delay_ms(5);
}

void lcd_clear_line(uint8_t line)
{
    lcd_command(line);
    for (uint8_t i = 0; i < 16; ++i) lcd_data(' ');
    lcd_command(line);
}

/* print unsigned int decimal (no leading zeros) */
void lcd_print_uint(uint32_t v)
{
    char buf[11]; // enough
    int idx = 0;
    if (v == 0) { lcd_data('0'); return; }
    while (v > 0) {
        buf[idx++] = (char)('0' + (v % 10));
        v /= 10;
    }
    while (idx--) lcd_data(buf[idx]);
}