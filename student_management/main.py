from operations import *

def menu():
    print("\n=================================\n")
    print("===STUDENT MANAGEMENT SYSTEM====")
    print("\n=================================\n")

    print("1-- Add student")
    print("\n2-- View Student")
    print("\n3-- Exit")

    ch = int(input("Choose your option: "))

    if(ch == 1):
        add_student()
    elif(ch == 2):
        view_student()
    elif(ch == 3):
        return
    else:
        print("Invalid choice...\n")
        return
    
menu()