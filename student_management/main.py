from operations import *

def menu():
    while(1):
        print("\n=================================\n")
        print("===STUDENT MANAGEMENT SYSTEM====")
        print("\n=================================\n")

        print("1-- Add student")
        print("2-- View Student")
        print("3-- Update Student")
        print("4-- Delete Student")
        print("5-- Search Student")
        print("0-- Exit")

        ch = int(input("\nChoose your option: "))

        if(ch == 1):
            add_student()
        elif(ch == 2):
            view_student()
        elif(ch == 3):
            update_student()
        elif(ch == 4):
            delete_student()
        elif(ch == 5):
            search_student()
        elif(ch == 0):
            break
        else:
            print("Invalid choice...\n")
            return
    
menu()