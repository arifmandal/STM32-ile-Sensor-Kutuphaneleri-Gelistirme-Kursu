import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QPushButton, QLabel, QFileDialog, QDesktopWidget, QLineEdit
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QMessageBox
from matplotlib.figure import Figure
import serial
import re

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.version = "1.0.0"
        self.copyright = "©2024 Arif Mandal"

        self.init_ui()

    def init_ui(self):
        self.setWindowTitle('Accelerometer Data')
        self.setGeometry(100, 100, 800, 600)

        self.setFixedSize(800, 600)

        self.com_port_edit = QLineEdit(self)
        self.baudrate_edit = QLineEdit(self)
        self.com_port_edit.setPlaceholderText("COM Port")
        self.baudrate_edit.setPlaceholderText("Baudrate")

        self.connect_button = QPushButton('Connect', self)
        self.connect_button.clicked.connect(self.connect_serial)

        self.disconnect_button = QPushButton('Disconnect', self)
        self.disconnect_button.clicked.connect(self.disconnect_serial)
        self.disconnect_button.setEnabled(False) 

        self.temp_label = QLabel('Temperature: ', self)
        self.humidity_label = QLabel('Humidity: ', self)
        self.bainc_label = QLabel('Altitude: ', self)

        self.fig = Figure()
        self.canvas = FigureCanvas(self.fig)
        self.ax = self.fig.add_subplot(111)

        layout = QVBoxLayout()
        layout.addWidget(self.com_port_edit)
        layout.addWidget(self.baudrate_edit)
        layout.addWidget(self.connect_button)
        layout.addWidget(self.disconnect_button)
        layout.addWidget(self.temp_label)
        layout.addWidget(self.humidity_label)
        layout.addWidget(self.bainc_label)
        layout.addWidget(self.canvas)
        
        self.launch_button = QPushButton('Launch Graph', self)
        self.launch_button.clicked.connect(self.show_plot)
        layout.addWidget(self.launch_button)

        self.stop_button = QPushButton('Stop Graph', self)
        self.stop_button.clicked.connect(self.stop_plot)
        layout.addWidget(self.stop_button)

        self.refresh_button = QPushButton('Refresh', self)
        self.refresh_button.clicked.connect(self.refresh_plot)
        layout.addWidget(self.refresh_button)

        self.save_button = QPushButton('Save', self)
        self.save_button.clicked.connect(self.save_data)
        layout.addWidget(self.save_button)


        version_label = QLabel(f"Versiyon: {self.version}", self)
        version_label.setAlignment(Qt.AlignCenter)
        copyright_label = QLabel(self.copyright, self)
        copyright_label.setAlignment(Qt.AlignCenter)

        layout.addWidget(version_label)
        layout.addWidget(copyright_label)
        
        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

        self.ser = serial.Serial('COM5', baudrate=9600)
        self.launch_button.setEnabled(False)
        self.stop_button.setEnabled(False)
        self.save_button.setEnabled(False)
        self.refresh_button.setEnabled(False)

        self.x_data = []
        self.y_data = []
        self.z_data = []

        self.plotting = True

    def connect_serial(self):
        com_port = self.com_port_edit.text()
        baudrate = self.baudrate_edit.text()

        try:
            baudrate = int(baudrate)
        except ValueError:
            self.show_error_message("Baudrate must be an integer.")
            return

        try:
            if self.ser and self.ser.is_open:
                self.ser.close()

            self.ser = serial.Serial(com_port, baudrate)
            if self.ser.is_open:
                print(f"Connection is successful: {com_port}, Baudrate: {baudrate}")
                self.show_info_message("Connected")

                self.launch_button.setEnabled(True)
                self.stop_button.setEnabled(True)
                self.save_button.setEnabled(True)
                self.refresh_button.setEnabled(True)
                self.disconnect_button.setEnabled(True)
            else:
                self.show_error_message(f"Connection is unsuccessful: {com_port}, Baudrate: {baudrate}")

        except serial.SerialException as e:
            self.show_error_message(f"Error connecting to {com_port}")
    def disconnect_serial(self):
        if self.ser and self.ser.is_open:
            self.show_info_message("Disconnected")
            self.ser.close()
            
            self.disconnect_button.setEnabled(False)
            self.connect_button.setEnabled(True)
            
            self.launch_button.setEnabled(False)
            self.stop_button.setEnabled(False)
            self.save_button.setEnabled(False)
            self.refresh_button.setEnabled(False)
            
            print("Disconnected")

    def show_error_message(self, message):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Critical)
        msg.setText(message)
        msg.setWindowTitle("Error")
        msg.exec_()

    def show_info_message(self, message):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Information)
        msg.setText(message)
        msg.setWindowTitle("Information")
        msg.exec_()

    def show_plot(self):
        try:
            while True:
                data = self.ser.readline().decode('utf-8').strip()

                match = re.match(r'x: (-?\d+), y: (-?\d+), z: (-?\d+)', data)
                if match:
                    x, y, z = map(int, match.groups())

                    temp_value = f'Temperature: {x}'
                    humidity_value = f'Humidity: {y}'
                    bainc_value = f'Altitude: {z}'

                    self.temp_label.setText(temp_value)
                    self.humidity_label.setText(humidity_value)
                    self.bainc_label.setText(bainc_value)

                    self.x_data.append(x)
                    self.y_data.append(y)
                    self.z_data.append(z)



                    self.ax.clear()
                    self.ax.plot(self.x_data, label='X', color='green')
                    self.ax.plot(self.y_data, label='Y', color='red')
                    self.ax.plot(self.z_data, label='Z', color='blue')

                    self.ax.legend(["X (Green)", "Y (Red)", "Z (Blue)"])

                    self.canvas.draw()

                    QApplication.processEvents()

                    if not self.plotting:
                        self.plotting = True
                        break

        except KeyboardInterrupt:
            print("Interrupted by user")
        finally:
            print("finally")
            #self.ser.close()

    def stop_plot(self):
        self.plotting = False

    
    def save_data(self):
        file_dialog = QFileDialog()
        file_path, _ = file_dialog.getSaveFileName(self, 'Dosya Kaydet', '', 'Text Files (*.txt)')

        if file_path:
            with open(file_path, 'w') as file:
                for i in range(len(self.x_data)):
                    file.write(f"{self.x_data[i]},{self.y_data[i]},{self.z_data[i]}\n")


    def refresh_plot(self):
        self.x_data = []
        self.y_data = []
        self.z_data = []
        self.ax.clear()
        self.ax.legend([])
        self.canvas.draw()

    def close_app(self):
        self.ser.close()
        self.close()

    def closeEvent(self, event):
        self.ser.close()
        event.accept()   

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())