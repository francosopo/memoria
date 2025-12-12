# INSTALL
Install gsl library with:
```sh
sudo apt install libgsl27 libgsl-dev
```
Install PostgreSQL, MariaDB y SQLite, besides its libraries with
```sh
sudo apt install postgresql-16
sudo apt install mariadb-server mariadb-client galera-4
sudo systemctl daemon-reload

sudo apt install sqlite3 libsqlite3-dev libpq-dev libmariadb3 libmariadb-dev make
```

Create the user, database and grant all permissions to the user inside that database

Create a config.h file and locate it inside src/include with the following variables:

```c
#define HOST
#define PSQL_PORT 
#define MARIADB_PORT 
#define USER 
#define PASSWORD 
#define DATABASE
```
# RUN

Execute 
```sh
make psql
make mariadb
make sqlite
```
The executables are named as follows:

1. psql_out
2. mariadb_out
3. sqlite_out

And the stats file follow the structure below:

avg [sec], std[sec], min[sec], max[sec], median[sec], var[sec]

Each line corresponds to 
INSERT, SELECT and UPDATE operations