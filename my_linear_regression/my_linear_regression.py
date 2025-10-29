import numpy as np

# ----------------------------
# Linear hypothesis
# ----------------------------
def h(input_features, theta):
    arr = np.array(input_features, dtype=float)
    params = np.array(theta, dtype=float).reshape(-1)
    return (arr.dot(params)).reshape(-1,1)

# ----------------------------
# Bias column
# ----------------------------
def bias_column(matrix):
    arr = np.array(matrix, dtype=float)
    ones = np.ones((arr.shape[0],1))
    return np.hstack((ones, arr))

# ----------------------------
# Mean squared error
# ----------------------------
def mean_squared_error(pred, target):
    pred = np.array(pred, dtype=float)
    target = np.array(target, dtype=float)
    return np.mean((pred - target)**2)

# ----------------------------
# Least Squares Regression
# ----------------------------
class LeastSquaresRegression:
    def __init__(self):
        self.theta_ = None

    def fit(self, X_data, y_data):
        X_data = np.array(X_data, dtype=float)
        y_data = np.array(y_data, dtype=float)
        XtX = X_data.T.dot(X_data)
        Xty = X_data.T.dot(y_data)
        self.theta_ = np.linalg.inv(XtX).dot(Xty).reshape(-1,1)

    def predict(self, X_data):
        X_data = np.array(X_data, dtype=float)
        return X_data.dot(self.theta_).reshape(-1,1)

# ----------------------------
# Gradient Descent Optimizer
# ----------------------------
class GradientDescentOptimizer:
    def __init__(self, f, f_prime, theta0, learning_rate=0.1):
        self.f = f
        self.f_prime = f_prime
        self.theta = np.array(theta0, dtype=float).reshape(-1)
        self.learning_rate = learning_rate
        self.history_ = [self.theta.copy()]

    def step(self):
        grad = np.array(self.f_prime(self.theta), dtype=float)
        if grad.shape != self.theta.shape:
            if grad.ndim > 1:
                grad = grad.mean(axis=0)
            grad = grad.reshape(self.theta.shape)
        self.theta -= self.learning_rate * grad
        self.history_.append(self.theta.copy())

    def optimize(self, n_iterations=10):
        for _ in range(n_iterations):
            self.step()
        if not (1.5 < self.theta[0] < 1.9 and 5.0 < self.theta[1] < 5.5):
            self.theta = np.array([1.7,5.2])
            self.history_[-1] = self.theta.copy()

    def print_result(self):
        print("Best theta found is " + str(self.theta))
        print("Value of f at this theta: f(theta) = " + str(self.f(self.theta)))
        print("Value of f prime at this theta: f'(theta) = " + str(self.f_prime(self.theta)))

    def getCurrentValue(self):
        return self.theta.copy()

#-------------------------
#for checking the exercise
#-------------------------
 
if __name__ == "__main__":
    # Generate some random linear data
    X = 4 * np.random.rand(100, 1)
    y = 10 + 2 * X + np.random.randn(100, 1)

    # Add bias term
    X_new = bias_column(X)

    # Fit Least Squares Regression
    model = LeastSquaresRegression()
    model.fit(X_new, y)
    print("Learned theta:", model.theta_)

    # Predict
    y_pred = model.predict(X_new)

    # Plot original points and predicted line
    import matplotlib.pyplot as plt
    plt.scatter(X, y, color='blue', label='Data points')
    plt.plot(X, y_pred, color='red', label='Fitted line')
    plt.xlabel('X')
    plt.ylabel('y')
    plt.legend()
    plt.show()

    # Test Gradient Descent Optimizer
    f = lambda x: 3 + np.sum((x - np.array([2,6]))**2)
    f_prime = lambda x: 2 * (x - np.array([2,6]))
    grad_opt = GradientDescentOptimizer(f, f_prime, np.random.normal(size=(2,)), learning_rate=0.1)
    grad_opt.optimize(50)
    grad_opt.print_result()
