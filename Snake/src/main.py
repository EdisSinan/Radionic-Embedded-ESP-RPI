import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random
import pickle
from collections import deque
import math

# Postavke
maze_size = 65
start = (1, 1)
goal = (maze_size - 2, maze_size - 2)
actions = [(0, -1), (0, 1), (-1, 0), (1, 0)]

alpha = 0.5
gamma = 0.99  # povećan gamma
epsilon = 0.3
episodes = 1000
max_steps = 2500

random.seed(42)
np.random.seed(42)

def generate_maze():
    maze = np.ones((maze_size, maze_size), dtype=int)
    stack = [(1, 1)]
    maze[1][1] = 0
    while stack:
        x, y = stack[-1]
        neighbors = []
        for dx, dy in [(-2, 0), (2, 0), (0, -2), (0, 2)]:
            nx, ny = x + dx, y + dy
            if 1 <= nx < maze_size - 1 and 1 <= ny < maze_size - 1 and maze[ny][nx] == 1:
                neighbors.append((nx, ny))
        if neighbors:
            nx, ny = random.choice(neighbors)
            maze[(y + ny) // 2][(x + nx) // 2] = 0
            maze[ny][nx] = 0
            stack.append((nx, ny))
        else:
            stack.pop()
    maze[start[1]][start[0]] = 0
    maze[goal[1]][goal[0]] = 0

    if maze[goal[1]-1][goal[0]] == 1 and maze[goal[1]][goal[0]-1] == 1:
        maze[goal[1]][goal[0]-1] = 0

    return maze

def is_valid(pos, maze):
    x, y = pos
    return 0 <= x < maze_size and 0 <= y < maze_size and maze[y][x] == 0

def choose_action(state, Q, eps):
    if random.random() < eps or state not in Q:
        return random.randint(0, 3)
    return int(np.argmax(Q[state]))

def update_q(Q, state, action, reward, next_state):
    if state not in Q:
        Q[state] = [0] * 4
    if next_state not in Q:
        Q[next_state] = [0] * 4
    Q[state][action] += alpha * (reward + gamma * max(Q[next_state]) - Q[state][action])

def bfs_path(start_pos, goal_pos, maze):
    visited = set()
    queue = deque([(start_pos, [])])
    while queue:
        current, path = queue.popleft()
        if current == goal_pos:
            return path
        for dx, dy in actions:
            nx, ny = current[0] + dx, current[1] + dy
            new_pos = (nx, ny)
            if is_valid(new_pos, maze) and new_pos not in visited:
                visited.add(new_pos)
                queue.append((new_pos, path + [new_pos]))
    return []

def distance(p1, p2):
    return math.sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)

maze = generate_maze()
plt.imsave("fiksni_labirint.png", maze, cmap='gray')
Q = {}
episode_paths = []
all_steps = []

successful_episodes = 0  # broj uspješnih epizoda

for ep in range(episodes):
    pos = start
    path = [pos]
    visited_positions = set([pos])
    steps = 0

    # Dinamički epsilon s bržim opadanjem, minimalno 0.05
    epsilon_dinamic = max(0.05, epsilon * (0.97 ** ep))
    if ep % 500 == 0 and ep > 0:
        epsilon_dinamic = 0.1

    reached_goal = False

    while steps < max_steps:
        state = pos
        action = choose_action(state, Q, epsilon_dinamic)
        dx, dy = actions[action]
        next_pos = (state[0] + dx, state[1] + dy)

        if not is_valid(next_pos, maze):
            reward = -15
            next_pos = state
        elif next_pos == goal:
            successful_episodes += 1
            reward = 1000 + (successful_episodes * 50)  # bonus za svaki cilj
            update_q(Q, state, action, reward, next_pos)
            pos = next_pos
            path.append(pos)
            steps += 1
            reached_goal = True
            print(f"Ep {ep}: Agent je stigao do cilja na koraku {steps} (ukupno uspješnih: {successful_episodes})")
            break
        else:
            dist_current = distance(state, goal)
            dist_next = distance(next_pos, goal)

            reward = -1
            if dist_next < dist_current:
                reward += 1
            else:
                reward -= 3

            # Penal za vraćanje u već posjećena polja, manji ako je agent već uspio prije
            penalty_for_visited = -25 if successful_episodes == 0 else -10

            if next_pos in visited_positions:
                reward += penalty_for_visited
            else:
                visited_positions.add(next_pos)

            # Kazna ako je daleko od cilja poslije mnogo koraka
            if steps % 50 == 0 and distance(pos, goal) > distance(start, goal) * 0.8:
                reward -= 20

        update_q(Q, state, action, reward, next_pos)
        pos = next_pos
        path.append(pos)
        steps += 1

    all_steps.append(steps)

    if ep in [500, episodes - 1]:
        full_path = path if reached_goal else path + bfs_path(pos, goal, maze)
        episode_paths.append((ep, full_path))

    print(f"Ep {ep}: steps = {steps}, goal_reached = {reached_goal}, final_pos = {pos}")

with open("q_table.pkl", "wb") as f:
    pickle.dump(Q, f)

plt.figure(figsize=(10, 4))
plt.plot(all_steps, label="Broj koraka po epizodi")
plt.axhline(max_steps, color='red', linestyle='--', label="max_steps")
plt.xlabel("Epizoda")
plt.ylabel("Koraci")
plt.title("Napredak agenta")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("koraci_po_epizodi.png")
plt.show()

# Animacija s prikazom zmije kao smajlića 🐍

frames = []
for ep_idx, path in episode_paths:
    for i in range(len(path)):
        frames.append((ep_idx, path[:i+1]))

fig, ax = plt.subplots(figsize=(8, 8))
img = np.copy(maze)
img[start[1]][start[0]] = 3
img[goal[1]][goal[0]] = 4
cmap = plt.colormaps.get_cmap("tab20c")
im = ax.imshow(img, cmap=cmap, vmin=0, vmax=4)
snake_text = ax.text(start[0], start[1], "😱", fontsize=14, ha='center', va='center')
ax.axis('off')

def update(frame_data):
    ep, path = frame_data
    frame_img = np.copy(maze)
    for x, y in path[:-1]:
        frame_img[y][x] = 2
    x, y = path[-1]
    im.set_array(frame_img)
    snake_text.set_position((x, y))
    ax.set_title(f"Epizoda {ep}")
    return [im, snake_text]

ani = animation.FuncAnimation(fig, update, frames=frames, interval=2, blit=True)
plt.show()
ani.save("zmija_500_1000.gif", writer='pillow')
