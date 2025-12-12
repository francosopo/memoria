import os
import matplotlib.pyplot as plt
from numpy import array
import pandas as pd

SRC_FOLDER = "../src"
sqlite_stats_file = os.path.join(SRC_FOLDER, "sqlite.csv")
maria_stats_file = os.path.join(SRC_FOLDER, "mariadb.csv")
post_stats_file = os.path.join(SRC_FOLDER, "psql.csv")

df_sqlite = pd.read_csv(sqlite_stats_file)
df_maria = pd.read_csv(maria_stats_file)
df_post = pd.read_csv(post_stats_file)

operation = ["Insert", "Select", "Update"]

sqlite_data = [ df_sqlite["avg"][0] * 1000, df_sqlite["avg"][1] * 1000, df_sqlite["avg"][2] * 1000]
sqlite_error = [ df_sqlite["std"][0] * 1000, df_sqlite["std"][1] * 1000, df_sqlite["std"][2] * 1000]

maria_data = [ df_maria["avg"][0] * 1000, df_maria["avg"][1] * 1000, df_maria["avg"][2] * 1000]
maria_error = [ df_maria["std"][0] * 1000, df_maria["std"][1] * 1000, df_maria["std"][2] * 1000]

post_data = [ df_post["avg"][0] * 1000, df_post["avg"][1] * 1000, df_post["avg"][2] * 1000]
post_error = [ df_post["std"][0] * 1000, df_post["std"][1] * 1000, df_post["std"][2] * 1000]

x = array([0, 5, 10])

bar_width = 0.5

fig, ax = plt.subplots()
ax.bar(x, sqlite_data, bar_width, label="SQLite")
ax.errorbar(x, sqlite_data, yerr=sqlite_error, fmt='o', color='k', capsize=3)
ax.bar(x + bar_width, maria_data, bar_width, label="MariaDB")
ax.errorbar(x + bar_width, maria_data, yerr=maria_error, fmt='o', color='k', capsize=3)
ax.bar(x + bar_width*2, post_data, bar_width, label="PostgreSQL")
ax.errorbar(x + bar_width*2, post_data, yerr=post_error, fmt='o', color='k', capsize=3)
ax.legend(loc='upper right', ncol=3)
ax.set_ylabel('Tiempo de latencia [ms]')
ax.set_xticks(x + bar_width, operation)
ax.xaxis.label.set_size(16)
ax.yaxis.label.set_size(16)
plt.tight_layout()
plt.title("Tiempo de latencia de las operaciones")
plt.savefig("plot_summary.png")

