import csv
import os
import sys

import matplotlib.pyplot as plt
import numpy


DATA_FOLDER = "../../stats/d"
BAR_WIDTH = 0.5
#OPERATIONS = ["INSERT", "SELECT", "UPDATE"]
OPERATIONS = ["SELECT", "UPDATE"]
# read_data(string,string) -> tuple(list, list)
# lee la data de un .csv y entrega un arreglo
# con el promedio y la desviación estándar
# respectiva al SGBD. 
def read_data(data_folder, dbname):
    filename = os.path.join(data_folder, dbname)
    try:
        f = open(filename,"r")
        csv_data = csv.reader(f)
        avg = [0,0,0]
        stdd = [0,0,0]
        i = 0
        for row in csv_data:
            if i > 0:
                avg[i - 1] = float(row[0]) * 1000
                stdd[i - 1] = float(row[1]) * 1000
            i += 1
        return (avg, stdd)
    finally:
        f.close()
    

def subplots(x, ax, avg, std, label):
    ax.bar(x, avg, BAR_WIDTH, label=label)
    ax.errorbar(x, avg, yerr=std, fmt='o', color='k', capsize=3)

psql_avg, psql_std = read_data(DATA_FOLDER, "psql.csv")
mariadb_avg, mariadb_std = read_data(DATA_FOLDER, "mariadb.csv")
sqlite_avg, sqlite_std = read_data(DATA_FOLDER, "sqlite.csv")

x = numpy.array([0,5])

fig, ax = plt.subplots()

subplots(x, ax, sqlite_avg[1:], sqlite_std[1:], "SQLITE")
subplots(x + BAR_WIDTH, ax, psql_avg[1:], psql_std[1:], "PostgreSQL")
subplots(x + BAR_WIDTH * 2, ax, mariadb_avg[1:], mariadb_std[1:], "MariaDB")


ax.set_title("Average execution time")
ax.legend(loc='upper right', ncol=3)
ax.set_ylabel('Time [ms]')
ax.set_xticks(x + BAR_WIDTH, OPERATIONS)
ax.xaxis.label.set_size(16)
ax.yaxis.label.set_size(16)
plt.ylim(0,0.04)
plt.tight_layout()
plt.savefig("plot_summary_zoom.png")

