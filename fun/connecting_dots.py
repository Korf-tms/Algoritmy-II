# Potential solution to Excercise 6.1.6 from Levitin
import numpy as np
import matplotlib.pyplot as plt


# center has average coordinates
def compute_center(points):
    return np.mean(points, axis=0)

# angle computed from a positive x-axis, atan2 does what we want
def sort_points_by_angle(points, center):
    def angle_from_center(point):
        return np.arctan2(point[1] - center[1], point[0] - center[0])
    return sorted(points, key=angle_from_center)

# Function to plot the points and the piecewise linear curve
def plot_curve(points, sorted_points, center):
    plt.figure(figsize=(8, 8))

    # Plot the original points
    points = np.array(points)
    plt.scatter(points[:, 0], points[:, 1], color='blue', label='Points')

    # Plot the center point
    plt.scatter(center[0], center[1], color='red', marker='x', label='Center')

    # Plot the sorted points as a piecewise linear curve
    sorted_points = np.array(sorted_points)
    plt.plot(sorted_points[:, 0], sorted_points[:, 1], color='green', label='Curve')

    # Close the loop
    plt.plot([sorted_points[-1, 0], sorted_points[0, 0]], [sorted_points[-1, 1], sorted_points[0, 1]], color='green')

    plt.legend()
    plt.title("Connecting Curve")
    plt.grid(True)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

def connect_points_with_curve(points):
    center = compute_center(points)
    sorted_points = sort_points_by_angle(points, center)
    plot_curve(points, sorted_points, center)

def generate_random_points(count):
    return np.random.randn(count, 2)


# Example usage
if __name__ == '__main__':
    points = generate_random_points(125)
    outlier = np.array([[10, 10]])  # many bracer to ensure suitable shape
    points = np.append(points, outlier, axis=0)
    connect_points_with_curve(points)
