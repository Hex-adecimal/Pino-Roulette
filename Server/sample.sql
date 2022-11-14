CREATE TABLE user ( 
    username TEXT PRIMARY KEY, 
    pswd TEXT NOT NULL
);

CREATE TABLE group ( 
    id ROWID, 
    group_name TEXT, 
    admin_username TEXT, 

    FOREIGN KEY (admin_username) REFERENCES user (username) 
);

CREATE TABLE relation_group_user (
    group_id INT PRIMARY KEY,
    username TEXT PRIMARY KEY,

    FOREIGN KEY (group_id) REFERENCES group (id),
    FOREIGN KEY (username) REFERENCES user (username) 
); 

CREATE TABLE message (
    sender TEXT,
    group_id INT,
    sent_time TEXT, /* "YYYY-MM-DD HH:MM:SS.SSS" */
    actual_message TEXT,

    FOREIGN KEY (group_id) REFERENCES group (id),
    FOREIGN KEY (sender) REFERENCES user (username) 
);

/* Request from user to enter into a group */
CREATE TABLE queue_for_group (
    user TEXT,
    id INT,

    FOREIGN KEY (user) REFERENCES user (username) 
)

INSERT INTO user VALUES ('pino.pompino', 'Miao');
INSERT INTO user VALUES ('effeo', 'Ricarica');

INSERT INTO group(group_name, admin_username) VALUES ('Gruppo A', 'pino.pompino');
INSERT INTO group(group_name, admin_username) VALUES ('Gruppo B', 'pino.pompino');

INSERT INTO relation_group_user VALUES (1, 'effeo');
INSERT INTO relation_group_user VALUES (1, 'pino.pompino');
INSERT INTO relation_group_user VALUES (2, 'pino.pompino');

INSERT INTO message VALUES ('pino.pompino', 1, '2001-11-13 23:34:00.000', '...');
