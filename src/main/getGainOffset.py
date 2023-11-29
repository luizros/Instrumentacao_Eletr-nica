import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def configData():
    df = pd.read_csv('../data/media.csv', sep=':')

    # Substituir vírgulas por pontos
    df = df.applymap(lambda x: str(x).replace(',', '.'))

    # Salvar o DataFrame de volta como CSV
    df.to_csv('seuarquivo_modificado.csv', index=False, sep=':')

    print("Valores modificados e salvos em 'seuarquivo_modificado.csv'")
