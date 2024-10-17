from screamer import lag_generator

def main():
    values = [10, 20, 30, 40]
    delay = 2
    initial = 0.0

    gen = lag_generator(values, delay, initial)
    for lagged in gen:
        print(lagged)

if __name__ == "__main__":
    main()
