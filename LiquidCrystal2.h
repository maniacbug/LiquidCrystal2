#ifndef LiquidCrystal2_h
#define LiquidCrystal2_h

#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/**
 * Lower-level LCD Hardware interface
 *
 * This interface should be provided by a driver that is able to interact
 * directly with an instance of LCD hardware.  The higher-level logic
 * class will use it to give instructions to the hardware.  This allows
 * you to use a port extender or shift register to drive the pins of the hardware.
 */

class ILcdHardware
{
public:
	/**
	 * Set the mode of the writes to follow
	 *
	 * @param mode Whether the following writes are commands (LOW) or data (HIGH)
	 */
	virtual void setMode(uint8_t mode) = 0;

	/**
	 * Write 4 bits to the hardware
	 */
	virtual void write4bits(uint8_t) = 0;

	/**
	 * Write 8 bits to the hardware
	 *
	 * In 4-bit mode, this function will handle splittling the nibbles
	 * correctly.
	 */
	virtual void write8bits(uint8_t) = 0;

	/**
	 * Test whether the hardware is wired for 4-bit transmission
	 *
	 * @return True if the hardware is in 4-bit, false in 8-bit
	 */
	virtual int fourbitmode() = 0;
};

/**
 * Low-level driver for LCD using a direct 6-11-pin connection.
 *
 * This code is all pulled out of the original LiquidCrystal class
 * supplied with Arduino.
 */

class LcdDirect: public ILcdHardware
{
public:
	LcdDirect(uint8_t rs, uint8_t enable,
				   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
				   uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
	LcdDirect(uint8_t rs, uint8_t rw, uint8_t enable,
				   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
				   uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
	LcdDirect(uint8_t rs, uint8_t rw, uint8_t enable,
				   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
	LcdDirect(uint8_t rs, uint8_t enable,
				   uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

	void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
			  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

	virtual void setMode(uint8_t);
	virtual void write4bits(uint8_t);
	virtual void write8bits(uint8_t);
	virtual int fourbitmode() { return _fourbitmode; }
private:
	void pulseEnable();

	/**
	 * Pin used for Register Select (RS)
	 *
	 * LOW = command mode, HIGH = data mode
	 */
	uint8_t _rs_pin;

	/**
	 * Pin used for Read/Write (RW)
	 *
	 * Set this to 255 if the pin is hard-wired
	 * to GND, which means we're permanantly in write mode.  LOW = write, HIGH =
	 * read.
	 */
	uint8_t _rw_pin;

	/**
	 * Pin used for Enable
	 *
	 * When this pin goes HIGH, the data and RS pins are read and acted upon.
	 */
	uint8_t _enable_pin; // activated by a HIGH pulse.

	/**
	 * Pins used for Data
	 *
	 * In four bit mode, only set _data_pins[0-3].  _data_pin[0] is the LSB of
	 * the data.  That's DB0 in 8-bit and DB4 in 4-bit.
	 */
	uint8_t _data_pins[8];

	/**
	 * Whether the hardware is wired for 4 bits
	 *
	 * HIGH = 4-bit, LOW = 8-bit.
	 */
	int _fourbitmode;
};

class LiquidCrystal2 : public Print {
public:
	/**
	 * Constructor
	 *
	 * @warning: This does not automatically 'begin' the LCD.  You still
  	 * have to do that when you're ready, in case your LcdHardware needs
  	 * to have an explicit begin() first.
  	 */
  LiquidCrystal2(ILcdHardware*);

  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t);
  virtual void write(uint8_t);
  void command(uint8_t);
protected:


private:
  void send(uint8_t, uint8_t);

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;
  uint8_t _numlines,_currline;

	ILcdHardware* _driver;
};

#endif
