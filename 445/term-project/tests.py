import auction as au
import time

au.beautify()
user1 = au.User("a@a", "testuser1", "testsurname1", "12345")
user1.verify(user1.email, user1.name+user1.email)

user3 = au.User("b@a", "testuser1", "testsurname1", "12345")
user3.verify(user3.email, user3.name+user3.email)
user3.addbalance(500)
user1.watch()

user1.watch("Sword")

user2 = au.User("b@b", "testuser2", "testsurname2", "12345")
user2.verify(user2.email, user1.name+user2.email)
user2.verify(user2.email, user2.name+user2.email)
user1.addbalance(100)
user1.addbalance(-20)
user1.changepassword("12345")
user1.changepassword("123456", "12345")
user1.report()


item1 = au.SellItem("b@b", "+9 Pala", "Sword",
                    "Guzel Item az kullanilmis", "increment", 100, 1)

item1.watch("a@a")

item2 = au.SellItem("b@b", "+8 Pala", "Sword",
                    "Guzel Item az kullanilmis", "increment", 100, 10)



item3 = au.SellItem("b@b", "+7 Pala", "Sword",
                    "Guzel Item az kullanilmis", "increment", 100, 10)

item4 = au.SellItem("b@b", "+6 Pala", "Sword",
                    "Guzel Item az kullanilmis", "increment", 100, 10)
time.sleep(2)

item1.view()
item1.startauction()
item1.view()
item2.startauction(5)
item2.startauction(20)
item4.startauction(100)

item1.bid("a@a", 200)
item1.bid("a@a", 40)
item3.bid("a@a",40)
item4.bid("b@a",300)

list1 = user1.listitems("b@b")

print("---Item List---")
for item in list1:
    print(item)
    print()

list2 = user1.listitems("b@b", "Sword")

print("---Item List---")
for item in list2:
    print(item)
    print()

list3 = user1.listitems("b@b", None, "active")

print("---Item List---")
for item in list3:
    print(item)
    print()

item1.sell()
item1.view()
item1.history()
user1.report()
user2.report()

while 1:
    pass
