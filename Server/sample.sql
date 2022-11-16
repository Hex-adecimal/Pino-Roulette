-- SQLite
CREATE TABLE user_t ( 
    username TEXT PRIMARY KEY, 
    pswd TEXT NOT NULL
);

CREATE TABLE group_t ( 
    id INTEGER PRIMARY KEY AUTOINCREMENT, 
    group_name TEXT NOT NULL, 
    admin_username TEXT NOT NULL, 

    FOREIGN KEY (admin_username) REFERENCES user_t (username) 
);

CREATE TABLE relation_group_user (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    group_id INT NOT NULL,
    username TEXT NOT NULL,

    FOREIGN KEY (group_id) REFERENCES group_t (id),
    FOREIGN KEY (username) REFERENCES user_t (username) 
); 

CREATE TABLE message (
    sender TEXT NOT NULL,
    group_id INTEGER NOT NULL,
    sent_time TEXT, /* "YYYY-MM-DD HH:MM:SS.SSS" */
    actual_message TEXT,

    FOREIGN KEY (group_id) REFERENCES group_t (id),
    FOREIGN KEY (sender) REFERENCES user_t (username) 
);

/* Request from user to enter into a group */
CREATE TABLE queue_for_group (
    user TEXT NOT NULL,
    id_group INTEGER NOT NULL,

    FOREIGN KEY (user) REFERENCES user_t (username),
    FOREIGN KEY (id_group) REFERENCES group_t (id) 
)

INSERT INTO user_t VALUES ('pino.pompino', 'Miao');
INSERT INTO user_t VALUES ('effeo', 'Ricarica');

INSERT INTO group_t(group_name, admin_username) VALUES ('Gruppo A', 'pino.pompino');
INSERT INTO group_t(group_name, admin_username) VALUES ('Gruppo B', 'pino.pompino');

INSERT INTO relation_group_user(group_id, username) VALUES (1, 'effeo');
INSERT INTO relation_group_user(group_id, username) VALUES (1, 'pino.pompino');
INSERT INTO relation_group_user(group_id, username) VALUES (2, 'pino.pompino');

INSERT INTO message VALUES ('pino.pompino', 1, '2001-11-13 23:34:00.000', '...');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:00.000', 'Lorem ipsum dolor sit amet,');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'consectetur adipiscing elit,');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'Ut enim ad minim veniam, ');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. ');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'Excepteur sint occaecat cupidatat non proident, ');
INSERT INTO message VALUES ('effeo', 1, '2022-11-16 16:20:01.000', 'sunt in culpa qui officia deserunt mollit anim id est laborum.');