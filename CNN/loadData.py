import numpy as np
import pandas as pd
from PIL import Image

TRAIN_FILE = '../database/training.csv'
TEST_FILE = '../database/test.csv'
SAVE_PATH = '../database/trained'


VALI_SIZE = 100    #size of valiadation set
EPOCHS = 100             #epoch limit
BATCH_SIZE = 64          #batch size
EARLY_STOP = 10 #early stopping


def input_data(test=False):
    file_name = TEST_FILE if test else TRAIN_FILE
    df = pd.read_csv(file_name)
    cols = df.columns[:-1]

    df = df.dropna()
    df['Image'] = df['Image'].apply(lambda img: np.fromstring(img, sep=' ') / 255.0)

    X = np.vstack(df['Image'])
    X = X.reshape((-1, 96, 96, 1))

    if test:
        y = None
    else:
        y = df[cols].values / 96.0
    return X, y




if __name__ == '__main__':
    print "data loaded"
