import pandas as pd
from matplotlib import pyplot as plt

data_folder= ".."

df_psql = pd.read_csv(f"{data_folder}/psql.csv")
df_mariadb = pd.read_csv(f"{data_folder}/mariadb.csv")
df_sqlite = pd.read_csv(f"{data_folder}/sqlite.csv")

def plot_df(the_df, other_df, df1_name, df2_name):
    scale= 1000
    df1 = pd.DataFrame({
        "operations":["GRABADO", "LECTURA", "ACTUALIZACIÓN"],
        "avg": the_df["avg"]*scale,
        "std": the_df["std"]*scale
    })
    df2 = pd.DataFrame({
        "operations": ["GRABADO", "LECTURA", "ACTUALIZACIÓN"],
        "avg": other_df["avg"]*scale,
        "std": other_df["std"]*scale
    })
    ax = df1.plot(style='o',x="operations", y="avg", yerr="std", label=df1_name, xlabel="Operaciones", ylabel="Tiempo [ms]", title="Tiempo de operaciones en bases de datos")
    df2.plot(style='o',ax=ax, x="operations", y="avg", yerr="std", label=df2_name, xlabel="Operaciones", ylabel="Tiempo [ms]", title="Tiempo de operaciones en bases de datos")
    plt.show()

plot_df(df_mariadb, df_psql, "MariaDB", "PostgreSQL")