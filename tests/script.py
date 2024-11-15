import matplotlib.pyplot as plt

# Read points from input.txt
input_points = []
with open('input.txt', 'r') as f:
    for line in f:
        x, y = map(float, line.split())
        input_points.append((x, y))

# Read points from output.txt
output_points = []
with open('output.txt', 'r') as f:
    for line in f:
        x, y = map(float, line.split())
        output_points.append((x, y))

# Extract x and y coordinates
input_x, input_y = zip(*input_points)
output_x, output_y = zip(*output_points)

# Create the plot
plt.scatter(input_x, input_y, color='blue', label='Input Points', zorder=5)
plt.plot(output_x, output_y, color='red', marker='o', label='Connected Output Points', zorder=10)

# Add labels and title
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Scatter Plot of Input Points and Connected Output Points')
plt.legend()

# Show the plot
plt.grid(True)
plt.show()
