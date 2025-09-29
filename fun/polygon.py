import numpy as np
from scipy.spatial import ConvexHull
import matplotlib.pyplot as plt

def calculate_distance(p1, p2):
    return np.sqrt(np.sum((p1 - p2)**2))

def find_closest_edge(polygon_points, point):
    """
    Find the edge that yields the least prolongation of edge length
    Returns: The index of the edge for insertion and the minimum cost.
    """
    min_cost = float('inf')
    best_edge_index = -1

    for i in range(len(polygon_points)):
        p1 = polygon_points[i]
        p2 = polygon_points[(i + 1) % len(polygon_points)]

        cost = calculate_distance(p1, point) + calculate_distance(point, p2) - calculate_distance(p1, p2)

        if cost < min_cost:
            min_cost = cost
            best_edge_index = i

    return best_edge_index, min_cost

def create_simple_polygon(points):
    """
    Returns a list of points in the order they appear in the simple polygon.
    Naive and unoptimized.
    """
    if len(points) < 3:
        return points.tolist()

    # Find the convex hull
    hull = ConvexHull(points)
    hull_indices = hull.vertices
    polygon_indices = list(hull_indices)

    # Identify interior points
    all_indices = set(range(len(points)))
    hull_set = set(hull_indices)
    interior_indices = list(all_indices - hull_set)

    # Iteratively add interior points
    while interior_indices:
        best_point_to_insert = -1
        best_insertion_index = -1
        min_total_cost = float('inf')
        
        polygon_points = [points[i] for i in polygon_indices]

        # Find the best point and edge to insert
        for point_idx in interior_indices:
            point_to_insert = points[point_idx]
            edge_index, cost = find_closest_edge(polygon_points, point_to_insert)

            if cost < min_total_cost:
                min_total_cost = cost
                best_point_to_insert = point_idx
                best_insertion_index = edge_index

        # Insert the best point into the polygon
        polygon_indices.insert(best_insertion_index + 1, best_point_to_insert)
        interior_indices.remove(best_point_to_insert)

    return [points[i] for i in polygon_indices]

def plot_polygon(points, polygon):
    plt.figure(figsize=(8, 8))
    
    plt.plot(points[:, 0], points[:, 1], 'o', label='Points')

    poly_points = np.array(polygon + [polygon[0]]) # Close the polygon for plotting
    plt.plot(poly_points[:, 0], poly_points[:, 1], '-r', label='Polygon Edges')

    plt.title('Polygon Construction')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.grid(True)
    plt.axis('equal')
    plt.show()

if __name__ == '__main__':
    # Generate a set of random points
    np.random.seed(11)
    num_points = 100
    points = np.random.rand(num_points, 2) * 10

    # Create the polygon?
    simple_polygon = create_simple_polygon(points)

    # Plot the result
    plot_polygon(points, simple_polygon)