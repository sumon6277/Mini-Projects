import mysql.connector

def get_connection():
    return mysql.connector.connect(
        host = 'localhost',
        user = 'root',
        passwd = 'password',
        database = 'student_management'
    )
