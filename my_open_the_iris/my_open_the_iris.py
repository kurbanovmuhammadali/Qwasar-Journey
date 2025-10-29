import pandas as pd
import matplotlib.pyplot as plt
from pandas.plotting import scatter_matrix
from sklearn.model_selection import train_test_split, cross_val_score, StratifiedKFold
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.svm import SVC

# Part I: Load data
def load_dataset():
    return pd.read_csv("iris.csv")

# Part II: Summarize dataset
def summarize_dataset(dataset):
    print("Dataset dimension:")
    print(dataset.shape)

    print("\nFirst 10 rows of dataset:")
    print(dataset.head(10))

    print("\nStatistical summary:")
    print(dataset.describe())

    print("\nClass Distribution:")
    print(dataset.groupby('class').size())

# Part III: Visualization
def print_plot_univariate(dataset):
    dataset.hist()
    plt.suptitle("Univariate Plots (Histogram)")
    plt.show()

def print_plot_multivariate(dataset):
    scatter_matrix(dataset)
    plt.suptitle("Multivariate Plots (Scatter Matrix)")
    plt.show()

# Part IV: Model evaluation
def my_print_and_test_models(dataset):
    array = dataset.values
    X = array[:, 0:4]
    y = array[:, 4]

    X_train, X_validation, Y_train, Y_validation = train_test_split(X, y, test_size=0.20, random_state=1)

    models = [
        ('DecisionTree', DecisionTreeClassifier()),
        ('GaussianNB', GaussianNB()),
        ('KNeighbors', KNeighborsClassifier()),
        ('LogisticRegression', LogisticRegression(solver='liblinear', multi_class='ovr')),
        ('LinearDiscriminant', LinearDiscriminantAnalysis()),
        ('SVM', SVC(gamma='auto'))
    ]

    for name, model in models:
        kfold = StratifiedKFold(n_splits=10, shuffle=True, random_state=1)
        cv_results = cross_val_score(model, X_train, Y_train, cv=kfold, scoring='accuracy')
        print(f"{name}: {cv_results.mean():.6f} ({cv_results.std():.6f})")

# Main Execution
if __name__ == "__main__":
    dataset = load_dataset()
    summarize_dataset(dataset)
    print_plot_univariate(dataset)
    print_plot_multivariate(dataset)
    my_print_and_test_models(dataset)
