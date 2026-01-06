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

        print("\nStudent added successfully\n")

    except Exception as e:
        print("Error ", e)
    finally:
        cursor.close()
        mydb.close()

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

        print("\n                  Student lists :")
        print("                 -----------------")

        for std in results:
            print(std)

    except Exception as e:
        print("Error ", e)
    finally:
        cursor.close()
        mydb.close()

def update_student():
    try:
        mydb = get_connection()
        cursor = mydb.cursor()

        roll = int(input("Enter roll no for update: "))
        cursor.execute("SELECT * FROM STUDENT WHERE ROLL = %s",(roll,))
        if not cursor.fetchone():
            print("Student not found!!!\n")
            return
            
        print("You can update only 2 options\n")
        print("1-- Name,Age\n2-- Phone,Email")

        ch = int(input("choose any: "))

        if(ch == 1):
            nfname = input("\nEnter new first_name: ")
            nlname = input("\nEnter new last_name: ")
            nage = int(input("Enter new age: "))

            sql = """UPDATE STUDENT SET first_name=%s,last_name = %s, age=%s WHERE ROLL = %s"""
            values = (nfname,nlname,nage,roll)

            cursor.execute(sql,values)
            mydb.commit()
            print("Updated successfully for roll no : ",roll)

        elif(ch == 2):
            nphone = int(input("\nEnter new phone number: "))
            nemail = input("\nEnter new email : ")

            sql = "UPDATE STUDENT SET PHONE = %s, EMAIL = %s WHERE ROLL = %s"
            values = (nphone,nemail,roll)

            cursor.execute(sql,values)
            mydb.commit()

            print("\nUpdated successfully for roll no : ",roll)
        else:
            print("\nWrong entry...")
    except Exception as e:
        print("Error: ",e)
    finally:
        cursor.close()
        mydb.close()

def delete_student():
    try:
        mydb = get_connection()
        cursor = mydb.cursor()

        roll = int(input("Enter roll no for deletion: "))
        ch = input("Are you sure to delete(y/n) ?? ")

        if ch.lower() == 'n':
            print("\nCancelled...\n")
            return

        cursor.execute("DELETE FROM STUDENT WHERE ROLL = %s",(roll,))
        mydb.commit()

        print("\nDeleted successfully for roll no ",roll)

    except Exception as e:
        print("Error : ",e)
    finally:
        cursor.close()
        mydb.close()

def search_student():
    try:
        mydb = get_connection()
        cursor = mydb.cursor()

        ch = int(input("\n1-- first_name\n2-- last_name\nChoose your option to search :"))

        if( ch == 1):
            fname = input("Enter first name to search : ")
            cursor.execute("SELECT * FROM STUDENT WHERE FIRST_NAME = %s",(fname,))

        elif(ch == 2):
            lname = input("Enter last name to search : ")
            cursor.execute("SELECT * FROM STUDENT WHERE LAST_NAME = %s",(lname,))
        else:
            print("\nInvalid choice...\n")
            return
        
        result = cursor.fetchall()
        if not result:
            print("\nStudent not found...\n")
            return
        else:
            print("\n                  Student lists :")
            print("                 -----------------")
            for row in result:
                print(row)

    except Exception as e:
        print("Error : ",e)
    finally:
        cursor.close()
        mydb.close()
        