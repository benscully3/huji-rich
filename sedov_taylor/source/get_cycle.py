def main():

    import h5py

    with h5py.File('output/final.h5') as f:
        print(f['cycle'])

if __name__ == '__main__':

    main()
