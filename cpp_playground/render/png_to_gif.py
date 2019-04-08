import imageio
import os

if __name__ == '__main__':
    frames = []
    with imageio.get_writer('render.gif', mode='I') as writer:
        for fname in os.listdir('.'):
            if fname.startswith('f') and fname.endswith('.png'):
                image = imageio.imread(fname)[::-1]
                writer.append_data(image)