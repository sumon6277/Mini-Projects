from database import get_connection

def add_student():
    try:
        mydb = get_connection()
        cursor = mydb.cursor()

        roll = input('\nEnter roll no : ')
        f_name = input('\nEnter first name : ')
        l_name = input('\nEnter last name : ')
        age = int(input('\nEnter age : '))
        phone = int(input('\nEnter mobile no : '))
        email = input('\nEnter email : ')

        cursor.execute("SELECT * FROM STUDENT WHERE EMAIL = %s",(email,))

        if cursor.fetchone():
            print("Email exists already... \nTry again with unique email")
            return
        
        sql = """INSERT INTO STUDENT(ROLL, FIRST_NAME, LAST_NAME,AGE,PHONE,EMAIL) VALUES (%s, %s, %s, %s, %s, %s)"""
        values = (roll,f_name,l_name,age,phone,email)

        cursor.execute(sql, values)
        mydb.commit()

        print("Student added successfully\n")

    except Exception as e:
        print("Error ", e)

def view_student():
    try:
        mydb = get_connection()
        cursor = mydb.cursor()

        roll = input("\nEnter roll no : ")

        cursor.execute("SELECT * FROM STUDENT WHERE ROLL = %s", (roll,))
        if not cursor.fetchone():
            print("Student not found or you entered a wrong entry...\n")
            return

        sql = "SELECT * FROM STUDENT WHERE ROLL = %s "
        values = (roll,)

        cursor.execute(sql, values)
        results = cursor.fetchall()

        print("\nStudent list \n")

        for std in results:
            print(std)

    except Exception as e:
        print("Error ", e)

