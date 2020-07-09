from customer import Customer

import psycopg2

from config import read_config
from messages import *

POSTGRESQL_CONFIG_FILE_NAME = "database.cfg"

"""
    Connects to PostgreSQL database and returns connection object.
"""


def connect_to_db():
    db_conn_params = read_config(
        filename=POSTGRESQL_CONFIG_FILE_NAME, section="postgresql")
    conn = psycopg2.connect(**db_conn_params)
    conn.autocommit = False
    return conn


"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""


def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]


"""
    Prints list of available commands of the software.
"""


def help():
    # prints the choices for commands and parameters
    print("\n*** Please enter one of the following commands ***")
    print("> help")
    print("> sign_up <email> <password> <first_name> <last_name> <plan_id>")
    print("> sign_in <email> <password>")
    print("> sign_out")
    print("> show_plans")
    print("> show_subscription")
    print("> subscribe <plan_id>")
    print("> watched_movies <movie_id_1> <movie_id_2> <movie_id_3> ... <movie_id_n>")
    print("> search_for_movies <keyword_1> <keyword_2> <keyword_3> ... <keyword_n>")
    print("> suggest_movies")
    print("> quit")


"""
    Saves customer with given details.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_up(conn, email, password, first_name, last_name, plan_id):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            "SELECT * FROM Customer WHERE email = %s;",
            (email,))
        customer = cur.fetchone()
        if customer is not None:
            raise Exception("Email is already taken.")
        cur.execute(
            "INSERT INTO customer (email, password, first_name, last_name, plan_id,session_count) VALUES (%s, %s,%s, %s, %s, %s);",
            (email, password, first_name, last_name, plan_id, "0"))
        conn.commit()
        is_done = True
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Retrieves customer information if email and password is correct and customer's session_count < max_parallel_sessions.
    - Return type is a tuple, 1st element is a customer object and 2nd element is the response message from messages.py.
    - If email or password is wrong, return tuple (None, USER_SIGNIN_FAILED).
    - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (customer, CMD_EXECUTION_SUCCESS).
    - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
"""


def sign_in(conn, email, password):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = None
    message = USER_SIGNIN_FAILED
    try:
        cur.execute(
            "SELECT * FROM Customer WHERE email = %s;",
            (email,))
        customer = cur.fetchone()
        if customer is None:
            raise Exception(USER_SIGNIN_FAILED)
        if customer[2] != password:
            raise Exception(USER_SIGNIN_FAILED)
        cur.execute(
            "SELECT * FROM plan WHERE plan_id = %s;",
            (customer[-1],))
        plan = cur.fetchone()
        if customer[-2] >= plan[-2]:
            message = USER_ALL_SESSIONS_ARE_USED
            raise Exception(USER_ALL_SESSIONS_ARE_USED)
        cur.execute(
            "Update Customer SET session_count=%s WHERE customer_id = %s;",
            (customer[-2]+1, customer[0]))
        conn.commit()
        is_done = Customer(
            customer[0],
            customer[1],
            customer[3],
            customer[4],
            customer[5]+1,
            customer[6])
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Signs out from given customer's account.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Decrement session_count of the customer in the database.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_out(conn, customer):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            "Update Customer SET session_count=%s WHERE customer_id = %s;",
            (customer.session_count-1, customer.customer_id))
        conn.commit()
        is_done = True
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Quits from program.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Remember to sign authenticated user out first.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def quit(conn, customer):
    # TODO: Implement this function
    is_done = True
    message = CMD_EXECUTION_SUCCESS
    if customer is not None:
        is_done, message = sign_out(conn, customer)
    return is_done, message


"""
    Retrieves all available plans and prints them.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print available plans and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Resolution|Max Sessions|Monthly Fee
    1|Basic|720P|2|30
    2|Advanced|1080P|4|50
    3|Premium|4K|10|90
"""


def show_plans(conn):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            "SELECT * FROM Plan ;")
        plans = cur.fetchall()
        print("#|Name|Resolution|Max Sessions|Monthly Fee")
        for plan in plans:
            for index, item in enumerate(plan):
                if index == 4:
                    print(item)
                    break
                print(item, end="|")
        message = CMD_EXECUTION_SUCCESS
        is_done = True
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Retrieves authenticated user's plan and prints it. 
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print the authenticated customer's plan and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Resolution|Max Sessions|Monthly Fee
    1|Basic|720P|2|30
"""


def show_subscription(conn, customer):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            "SELECT * FROM plan WHERE plan_id = %s;",
            (customer.plan_id,))
        plan = cur.fetchone()
        print("#|Name|Resolution|Max Sessions|Monthly Fee")
        for index, item in enumerate(plan):
            if index == 4:
                print(item)
                break
            print(item, end="|")
        is_done = True
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Insert customer-movie relationships to Watched table if not exists in Watched table.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If a customer-movie relationship already exists, do nothing on the database and return (True, CMD_EXECUTION_SUCCESS).
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any one of the movie ids is incorrect; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def watched_movies(conn, customer, movie_ids):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        for movie_id in movie_ids:
            cur.execute(
                "SELECT * FROM movies WHERE movie_id = %s;",
                (movie_id,))
            movie = cur.fetchone()
            if movie is None:
                raise Exception("Movie Not Found")
            cur.execute(
                """INSERT INTO watched (customer_id, movie_id) 
                    SELECT %s,%s
                        WHERE NOT EXISTS (
                        SELECT
                            customer_id, movie_id
                        FROM
                            watched
                        WHERE
                            movie_id = %s
			                AND customer_id = %s);""",
                (customer.customer_id, movie_id, movie_id, customer.customer_id))
        conn.commit()
        is_done = True
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Subscribe authenticated customer to new plan.
    - Return type is a tuple, 1st element is a customer object and 2nd element is the response message from messages.py.
    - If target plan does not exist on the database, return tuple (None, SUBSCRIBE_PLAN_NOT_FOUND).
    - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
    - If the operation is successful, commit changes and return tuple (customer, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
"""


def subscribe(conn, customer, plan_id):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = None
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            "SELECT * FROM plan WHERE plan_id = %s;",
            (customer.plan_id,))
        current_plan = cur.fetchone()
        cur.execute(
            "SELECT * FROM plan WHERE plan_id = %s;",
            (plan_id,))
        plan = cur.fetchone()
        if plan is None:
            message = SUBSCRIBE_PLAN_NOT_FOUND
            raise Exception(SUBSCRIBE_PLAN_NOT_FOUND)
        if plan[-2] < current_plan[-2]:
            message = SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE
            raise Exception(SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE)
        cur.execute(
            "Update Customer SET plan_id=%s WHERE customer_id = %s;",
            (plan_id, customer.customer_id))
        conn.commit()
        customer.plan_id = plan_id
        is_done = customer
        message = CMD_EXECUTION_SUCCESS
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Searches for movies with given search_text.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Print all movies whose titles contain given search_text IN CASE-INSENSITIVE MANNER.
    - If the operation is successful; print movies found and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    Id|Title|Year|Rating|Votes|Watched
    "tt0147505"|"Sinbad: The Battle of the Dark Knights"|1998|2.200000|149|0
    "tt0468569"|"The Dark Knight"|2008|9.000000|2021237|1
    "tt1345836"|"The Dark Knight Rises"|2012|8.400000|1362116|0
    "tt3153806"|"Masterpiece: Frank Millers The Dark Knight Returns"|2013|7.800000|28|0
    "tt4430982"|"Batman: The Dark Knight Beyond"|0|0.000000|0|0
    "tt4494606"|"The Dark Knight: Not So Serious"|2009|0.000000|0|0
    "tt4498364"|"The Dark Knight: Knightfall - Part One"|2014|0.000000|0|0
    "tt4504426"|"The Dark Knight: Knightfall - Part Two"|2014|0.000000|0|0
    "tt4504908"|"The Dark Knight: Knightfall - Part Three"|2014|0.000000|0|0
    "tt4653714"|"The Dark Knight Falls"|2015|5.400000|8|0
    "tt6274696"|"The Dark Knight Returns: An Epic Fan Film"|2016|6.700000|38|0
"""


def search_for_movies(conn, customer, search_text):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            """select *
                from movies
                where LOWER(title) like LOWER(%s)
                order by movie_id""",
            ("%"+search_text+"%",))
        movies = cur.fetchall()
        print("Id|Title|Year|Rating|Votes|Watched")
        for movie in movies:
            cur.execute(
                "SELECT * FROM watched WHERE customer_id = %s and movie_id = %s;",
                (customer.customer_id, movie[0]))
            movie += (0,) if cur.fetchone() is None else (1, 0)
            for index, item in enumerate(movie):
                if index == 5:
                    print(item)
                    break
                print(item, end="|")
        message = CMD_EXECUTION_SUCCESS
        is_done = True
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message


"""
    Suggests combination of these movies:
        1- Find customer's genres. For each genre, find movies with most number of votes among the movies that the customer didn't watch.

        2- Find top 10 movies with most number of votes and highest rating, such that these movies are released 
           after 2010 ( [2010, today) ) and the customer didn't watch these movies.
           (descending order for votes, descending order for rating)

        3- Find top 10 movies with votes higher than the average number of votes of movies that the customer watched.
           Disregard the movies that the customer didn't watch.
           (descending order for votes)

    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.    
    - Output format and return format are same with search_for_movies.
    - Order these movies by their movie id, in ascending order at the end.
    - If the operation is successful; print movies suggested and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
"""


def suggest_movies(conn, customer):
    # TODO: Implement this function
    cur = conn.cursor()
    is_done = False
    message = CMD_EXECUTION_FAILED
    try:
        cur.execute(
            """select *
                from watched
                where customer_id = %s""",
            (customer.customer_id,))
        watched = cur.fetchall()
        genres = set()
        recoms = set()
        for movie in watched:
            cur.execute(
                "SELECT * FROM genres WHERE movie_id = %s;",
                (movie[1],))
            genre = cur.fetchall()
            for i in genre:
                genres.add(i[1])
        for genre in genres:
            cur.execute(
                """select m.movie_id,m.title,m.movie_year,m.rating,m.votes 
                            FROM movies m,genres g
                            where m.movie_id = g.movie_id and g.genre_name=%s AND m.movie_id NOT in(
                                                                        SELECT
                                                                            movie_id FROM watched
                                                                        WHERE
                                                                            customer_id = %s)
                            order by votes DESC
                            limit 1
                            """, (genre, customer.customer_id))
            recoms.add(cur.fetchone())
        cur.execute("""SELECT *
                    FROM movies
                    WHERE movie_year > 2009 AND movie_id NOT in(
                            SELECT
                                movie_id FROM watched
                            WHERE
                                customer_id = %s)
                    ORDER BY
                        votes DESC, rating DESC
                    LIMIT 10""", (customer.customer_id,))
        tops = cur.fetchall()
        for i in tops:
            recoms.add(i)
        cur.execute("""SELECT *
                        FROM movies
                        WHERE movie_id NOT in(
                                SELECT
                                    movie_id FROM watched
                                WHERE
                                    customer_id = %s)
                            AND votes > (
                                SELECT
                                    avg(votes)
                                FROM
                                    movies m, watched w
                                WHERE
                                    m.movie_id = w.movie_id
                                    AND w.customer_id = %s)
                            ORDER BY
                                votes DESC
                            LIMIT 10""", (customer.customer_id, customer.customer_id))
        votes = cur.fetchall()
        for i in votes:
            recoms.add(i)
        recoms = sorted(recoms)
        print("Id|Title|Year|Rating|Votes")
        for movie in recoms:
            for index, item in enumerate(movie):
                if index == 4:
                    print(item)
                    break
                print(item, end="|")
        message = CMD_EXECUTION_SUCCESS
        is_done = True
    except Exception as e:
        conn.rollback()
    finally:
        cur.close()
    return is_done, message
