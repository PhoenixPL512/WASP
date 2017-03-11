#!/usr/bin/python3
import argparse
from cansat_parser import CanSatParser

def main():
    data_parser = CanSatParser()
    parser = argparse.ArgumentParser(description="CanSat data analyser")
    parser.add_argument("-f", "--file", help="Path to file with log data. Leave empty for on-line reading.")
    parser.parse_args(namespace=data_parser)
    data_parser.start()

    data_parser.data.dump_to_file()

if __name__ == "__main__":
    main()
