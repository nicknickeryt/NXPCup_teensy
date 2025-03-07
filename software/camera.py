import pyqtgraph as pg
import numpy as np
from pyqtgraph.Qt import QtCore
import serial

# Serial configuration
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 115200  
serial_port = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

graphics_layout = pg.GraphicsLayoutWidget(show=True, title="Kitty")

plot0 = graphics_layout.addPlot(row=0, col=0, title="Wykres")
plot1 = graphics_layout.addPlot(row=1, col=0, title="Obraz")

graphics_layout.ci.layout.setRowStretchFactor(0, 4) 
graphics_layout.ci.layout.setRowStretchFactor(1, 1) 

graphics_layout.setBackground("#f1f1f1")

plot0.setLabel('left', 'Value')
plot0.setLabel('bottom', 'Sample')
plot0.showGrid(x=True, y=True)

x = np.arange(128)

plot0.setYRange(0, 256)
plot0.setXRange(0, 128)

line0 = plot0.plot(pen='r')

img = np.ones((128, 1))  
img_item = pg.ImageItem(img) 
plot1.addItem(img_item)  
plot1.getAxis('left').setVisible(False)

data = [0] * 128

def read_serial_data(serial_port):
    """Reads data from UART and returns a list of 128 values."""
    try:
        if serial_port.in_waiting > 0:
            line = serial_port.readline().decode('utf-8', errors='ignore').strip()
            if line.startswith("CAML."):
                data_str = line[5:]  # Remove "CAML."
                values = list(map(int, data_str.split('.')))
                if len(values) == 128:
                    return values
    except ValueError:
        print("⚠ Error parsing data:", line)
    return None

def update_plot():
    global data
    data = read_serial_data(serial_port)

    if data:
        line0.setData(x, data)
        
        img[:, 0] = data
        img_item.setImage(img)

timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(0)  

# Start the Qt event loop
pg.QtCore.QCoreApplication.instance().exec()

serial_port.close()

