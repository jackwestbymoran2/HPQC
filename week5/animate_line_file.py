import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import pandas as pd
import math as m
import random

from pathlib import Path
import sys

import argparse


# parses command line arguments for runtime configuration
def parse_args():
    parser = argparse.ArgumentParser(description="animate string file")

    # input csv file containing simulation data
    parser.add_argument("input_file", help="input csv file")

    # output animation file (gif or mp4)
    parser.add_argument("output_file", help="output animation file")

    # frames per second for saved animation
    parser.add_argument("--fps", type=int, default=30, help="frames per second")

    # time interval between animation frames in milliseconds
    parser.add_argument("--interval", type=int, default=50, help="frame interval")

    # optional number of frames to animate
    parser.add_argument("--frames", type=int, default=None, help="number of frames")

    return parser.parse_args()


# checks that input file has correct extension
def check_file(filename, extension):
    if filename.split('.')[-1] != extension:
        print("error: file {} is not a {} file".format(filename, extension))
        exit(-1)
    return filename


def main():
    # get command line arguments
    args = parse_args()

    # validate input file type
    args.input_file = check_file(args.input_file, "csv")

    # load simulation data from file
    data, num_positions, num_times = get_data(args.input_file)

    # configure animation parameters
    times, interval, fps = configure_animation(frame_count=num_times)

    # override defaults with user input if provided
    interval = args.interval
    fps = args.fps

    # extract x and y coordinates of string positions
    x_positions, y_positions = extract_position(data)

    # setup initial plot
    fig, rope = configure_plot(x_positions, y_positions)

    # determine number of frames to animate
    num_frames = args.frames if args.frames is not None else num_times

    # create animation object
    ani = animation.FuncAnimation(
        fig,
        animate,
        num_frames,
        interval=interval,
        blit=True,
        fargs=(data, rope)
    )

    # save animation to file
    ani.save(filename=args.output_file, writer="pillow", fps=fps)

    # confirm completion
    print("animation saved to {}".format(args.output_file))


if __name__ == "__main__":
    main()
