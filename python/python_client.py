from __future__ import print_function

"""
serial_writer.py

Send 1-byte number to a serial port.
Supports dummy port for testing without actual device.

Kyuhwa Lee, 2016
EPFL

"""

import sys
import serial, serial.tools.list_ports

class SerialWriter:
	def __init__(self, serial_port, baud_rate=115200):
		"""
		Input
		-----
		serial_port: int | string | None
			int: use a fixed COM port number.
			string: search with partial device name.
			None: show ports list and create a dummy port.
		baud_rate: int
			baud rate for serial communication

		Note
		----
		Arduino Mega and Micro can be searched with device name but
		Nano doesn't seem to work.
		"""
		self.comport= None

		if serial_port is None:
			self.list_ports()
		elif type(serial_port)==str:
			ports= [x for x in serial.tools.list_ports.grep(serial_port)]
			if len(ports) == 0:
				raise RuntimeError, 'Cannot find %s'% serial_port
			else:
				if len(ports) > 1:
					print('WARNING: More than one device detected.')
				for i,a in enumerate(ports):
					print('Found %s'% a )
				# just use the first one
				self.comport= ports[0].device
		elif type(serial_port)==int:
			self.comport= 'COM%d'% serial_port
		else:
			raise RuntimeError, 'Unrecognized name %s %s'% (serial_port,type(serial_port))

		if self.comport is not None:
			self.ser= serial.Serial(self.comport, baud_rate)
			print('Connected to %s.'% self.comport)
		else:
			self.ser= None
			self.write= self.write_dummy
			self.close= self.close_dummy
			print('**** WARNING: Connected to a dummy serial port. ****\n')

	def list_ports(self):
		print('--- List of active COM ports ---')
		for p in [x.description for x in serial.tools.list_ports.comports()]:
			print(p)
		print()

	def write(self, number):
		return self.ser.write( chr(number) )

	def close(self):
		print('\nClosing serial port.')
		self.ser.close()
		self.ser= None

	def is_dummy(self):
		return self.ser is None

	def write_dummy(self, number):
		return len( chr(number) )

	def close_dummy(self):
		print('\nClosing dummy serial port.')

# sample code
if __name__ == '__main__':
	SERIAL_PORT= 'Arduino Micro'
	#SERIAL_PORT= None # test with a dummy device
	BAUD_RATE= 115200

	sw= SerialWriter(SERIAL_PORT, BAUD_RATE)

	print('Enter an integer between 0 and 255. Ctrl+C to stop.')
	while True:
		try:
			data= int( raw_input('Input? ').strip() )
			assert 0 <= data <= 255
			print('Sent', data)
		except KeyboardInterrupt:
			break

	sw.close()
	print('Done.')
