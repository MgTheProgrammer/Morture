import sys
from PyQt5.QtWidgets import (
    QMainWindow, QApplication,QLabel,QWidget,QTableWidget,QTableWidgetItem,QFormLayout,QGridLayout,QPushButton,QDialog,QLineEdit,QAction,QDialogButtonBox
)
from PyQt5.QtGui import QIcon
from PyQt5 import QtCore,QtWidgets
import util
import itertools
from client import my_client
import protocol

class holdStats(QTableWidget):
    def __init__(self, boardVals):
        super().__init__()
        self.setEditTriggers(QTableWidget.NoEditTriggers)
        self.setRowCount(len(boardVals))
        self.setColumnCount(len(boardVals[0]))

        self.setHorizontalHeaderLabels(list(itertools.islice(util.iter_all_strings(),len(boardVals[0]))))
        self.setVerticalHeaderLabels(list(reversed([str(i) for i in range(1, len(boardVals)+1)])))

        self.horizontalHeader().setSectionResizeMode(QtWidgets.QHeaderView.Stretch)
        self.verticalHeader().setSectionResizeMode(QtWidgets.QHeaderView.Stretch)

        self.setMinimumSize(self.columnWidth(0)*len(boardVals[0]),self.rowHeight(0)*len(boardVals))
        for i in range(len(boardVals)):
            for j in range(len(boardVals[0])):
                cellVal = QTableWidgetItem(str(boardVals[i][j]))
                cellVal.setTextAlignment(QtCore.Qt.AlignCenter)
                self.setItem(i,j,cellVal)
        

    def change(self,boardVals):
        for i in range(len(boardVals)):
            for j in range(len(boardVals[0])):
                cellVal = QTableWidgetItem(str(boardVals[i][j]))
                cellVal.setTextAlignment(QtCore.Qt.AlignCenter)
                self.setItem(i,j,cellVal)

class generalStats(QWidget):
    # TODO: write hints on hover
    UPTIME_HINT = "The number of microseconds passed since last reboot"
    NUM_CONNS_HINT = "The number of ble connections ever ocurred"
    NUM_ROUTES_HINT = "The number of routes Morture ever lit"
    NUM_LIT_RED_HINT = "The number of holds lit in red"
    NUM_LIT_GREEN_HINT = "The number of holds lit in green"
    NUM_LIT_BLUE_HINT = "The number of holds lit in blue"

    RED_COLOR = "#d9827c"
    GREEN_COLOR = "#BFC8AD"
    BLUE_COLOR = "#718F94"
    def __init__(self,Uptime,NumConns,NumRoutes,NumLitRed,NumLitGreen,NumLitBlue):
        super().__init__()
        layout = QFormLayout()
        self.setLayout(layout)
        layout.setLabelAlignment(QtCore.Qt.AlignLeft)

        self.UptimeLabel = QLabel(Uptime)
        self.NumConnsLabel = QLabel(NumConns)
        self.NumRoutesLabel = QLabel(NumRoutes)
        self.NumLitRedLabel = QLabel(NumLitRed)
        self.NumLitGreenLabel = QLabel(NumLitGreen)
        self.NumLitBlueLabel = QLabel(NumLitBlue)

        self.UptimeLabel.setToolTip(self.UPTIME_HINT)
        self.NumConnsLabel.setToolTip(self.NUM_CONNS_HINT)
        self.NumRoutesLabel.setToolTip(self.NUM_ROUTES_HINT)
        self.NumLitRedLabel.setToolTip(self.NUM_LIT_RED_HINT)
        self.NumLitGreenLabel.setToolTip(self.NUM_LIT_GREEN_HINT)
        self.NumLitBlueLabel.setToolTip(self.NUM_LIT_BLUE_HINT)
        
        self.NumLitRedLabel.setStyleSheet(f"color:{self.RED_COLOR};")
        self.NumLitGreenLabel.setStyleSheet(f"color:{self.GREEN_COLOR};")
        self.NumLitBlueLabel.setStyleSheet(f"color:{self.BLUE_COLOR};")

        layout.addRow("Uptime:",self.UptimeLabel)
        layout.addRow("Num connections:",self.NumConnsLabel)
        layout.addRow("Num routes",self.NumRoutesLabel)
        layout.addRow("Num lit red:",self.NumLitRedLabel)
        layout.addRow("Num lit green:",self.NumLitGreenLabel)
        layout.addRow("Num lit blue:",self.NumLitBlueLabel)

    def change(self,my_client:my_client):
        uptime = my_client.uptime()
        num_conns = my_client.num_conns()
        num_routes = my_client.num_routes()
        num_lit_red = my_client.num_color_lit(protocol.color.RED)
        num_lit_green = my_client.num_color_lit(protocol.color.GREEN)
        num_lit_blue = my_client.num_color_lit(protocol.color.BLUE)
        self.UptimeLabel.setText(uptime)
        self.NumConnsLabel.setText(num_conns)
        self.NumRoutesLabel.setText(num_routes)
        self.NumLitRedLabel.setText(num_lit_red)
        self.NumLitGreenLabel.setText(num_lit_green)
        self.NumLitBlueLabel.setText(num_lit_blue)

class MainWindow(QMainWindow):
    def __init__(self,my_client : my_client):
        super(MainWindow, self).__init__()
        self.my_client = my_client
        self.setWindowTitle("Morture")


        g = my_client.num_times_holds_lit()
        print("g: ",g)
        self.board = holdStats(g)
        placeholder_text = "NaN"
    
        self.general_stats_widget= generalStats(placeholder_text,placeholder_text,placeholder_text,placeholder_text,placeholder_text,placeholder_text)
        self.general_stats_widget.change(my_client)
        wid = QWidget()
        grid = QGridLayout()
        refresh_btn = QPushButton("Refresh values")
        refresh_btn.clicked.connect(self.refresh)
        
        grid.addWidget(self.board,0,1)
        grid.addWidget(self.general_stats_widget,0,0)
        grid.addWidget(refresh_btn,1,0,1,2)
        
        change_password_action = QAction(QIcon('new.png'), '&Change password', self)        
        change_password_action.triggered.connect(self.change_password)

        menuBar = self.menuBar()
        fileMenu = menuBar.addMenu('&Account')
        fileMenu.addAction(change_password_action)
        wid.setLayout(grid)
        self.setCentralWidget(wid)
        # g.change()
    def refresh(self):
        self.general_stats_widget.change(self.my_client)
        new_board_vals= self.my_client.num_times_holds_lit()
        self.board.change(new_board_vals)
    def change_password(self):
        dialog = changePasswordDialog(self.my_client)
        dialog.exec()

class changePasswordDialog(QDialog):
    def __init__(self,my_client : my_client):
        super().__init__()
        self.setWindowTitle("Change password")
        self.layout = QFormLayout()
        self.my_client = my_client

        self.lineEdit_username = QLineEdit()
        self.lineEdit_old_password = QLineEdit()
        self.lineEdit_new_password = QLineEdit()
        self.label_outcome = QLabel("")
        self.label_outcome.setAlignment(QtCore.Qt.AlignCenter)
        self.lineEdit_username.setPlaceholderText('Please enter your username')
        self.lineEdit_old_password.setPlaceholderText('Please enter your current password')
        self.lineEdit_new_password.setPlaceholderText('Please enter your new password')

        self.layout.addRow("Username:",self.lineEdit_username)
        self.layout.addRow("Old Password:",self.lineEdit_old_password)
        self.layout.addRow("New Password:",self.lineEdit_new_password)
        
        login_button = QPushButton("Change password")
        login_button.clicked.connect(self.change_password)
        self.layout.addWidget(login_button)
        self.layout.addRow(self.label_outcome)
        self.setLayout(self.layout)

        self.buttonBox = QDialogButtonBox(QDialogButtonBox.StandardButton.Cancel)
        self.buttonBox.rejected.connect(self.reject)
        self.layout.addWidget(self.buttonBox)
        
    def change_password(self):
        username = self.lineEdit_username.text()
        old_password = self.lineEdit_old_password.text()
        new_password = self.lineEdit_new_password.text()
        if self.my_client.change_password(username,old_password,new_password):
            self.accept()
        else:
            self.label_outcome.setText("Something went wrong please try again")

class loginDialog(QDialog):
    def __init__(self,my_client : my_client):
        super().__init__()
        self.my_client = my_client
        self.setWindowTitle("Login")
        self.layout = QFormLayout()

        self.lineEdit_username = QLineEdit()
        self.lineEdit_password = QLineEdit()
        self.label_outcome = QLabel("")
        self.label_outcome.setAlignment(QtCore.Qt.AlignCenter)
        self.lineEdit_username.setPlaceholderText('Please enter your username')
        self.lineEdit_password.setPlaceholderText('Please enter your password')

        self.layout.addRow("Username:",self.lineEdit_username)
        self.layout.addRow("Password:",self.lineEdit_password)
        
        login_button = QPushButton("login")
        login_button.clicked.connect(self.checkCredentials)
        self.layout.addWidget(login_button)
        self.layout.addRow(self.label_outcome)
        self.setLayout(self.layout)

    def checkCredentials(self):
        username = self.lineEdit_username.text()
        password = self.lineEdit_password.text()
        if self.my_client.login(username,password):
            self.accept()
        else:
            self.label_outcome.setText("Credentials are wrong please try again")


def main():
    app = QApplication(sys.argv)
    c = my_client("192.168.4.1",7050)
    l = loginDialog(c)
    l.exec()
    w = MainWindow(c)
    w.show()
    app.exec()
if __name__ == "__main__":
    main()
