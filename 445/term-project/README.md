# Online Bidding System 

Furkan Doğan / 2098937 

Sinan Talha Koşar / 2099190

First we created MongoDB database with replicas because watch funcionality of MongoDB is only for replica sets.

```
mkdir -p ~/data/mongodb/rs0-0 ~/data/mongodb/rs0-1 ~/data/mongodb/rs0-2

mongod --replSet rs0 --port 27017 --bind_ip localhost --dbpath ~/data/mongodb/rs0-0  --oplogSize 128
mongod --replSet rs0 --port 27018 --bind_ip localhost --dbpath ~/data/mongodb/rs0-1  --oplogSize 128
mongod --replSet rs0 --port 27019 --bind_ip localhost --dbpath ~/data/mongodb/rs0-2  --oplogSize 128
```

Then we connected MongoDB from terminal with this command:
```
mongo
````

And we first initiated the replica sets and then created database named bidding and two collections for users and items.
```
rs.initiate()
use bidding
db.createCollection("users")
db.createCollection("items")
```

In python we used pymongo which is official api like below to reach MongoDB

```python
client = pymongo.MongoClient("mongodb://localhost:27017")
db = client.get_database("bidding")
user_collection = db.get_collection("users")
item_collection = db.get_collection("items")
```

To encrypt the password before saving and the checked with the db we used passlib

```python
pwd_context = CryptContext(
    schemes=["pbkdf2_sha256"],
    default="pbkdf2_sha256",
    pbkdf2_sha256__default_rounds=30000
)

pwd_context.hash(password)

pwd_context.verify(password, hashed)
```

And to watch items we used threads and watch functionality of MongoDB

```python
_thread.start_new_thread(watch, (pipeline,))

def watch(pipeline):
    try:
        resume_token = None
        with item_collection.watch(pipeline)as stream:
            for change in stream:
                print(change)
                resume_token = stream.resume_token
    except pymongo.errors.PyMongoError:
        if resume_token is None:
            print("Failure during ChangeStream initialization.")
        else:
            with item_collection.watch(pipeline, resume_after=resume_token) as stream:
                for change in stream:
                    print(change)
```

In the tests.py file we tried to test all the usecases. You can start it by typing 

```
python3 tests.py
```

And you will able to see the results on the terminal and the database.