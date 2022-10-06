def main():

    import numpy
    import h5py
    import pylab

    with h5py.File('output/final.h5', 'r') as f:
        for n, field in enumerate(['density', 'pressure', 'x_velocity']):
            pylab.subplot(3,1,n+1)
            pylab.semilogx(numpy.array(f['grid']),
                           numpy.array(f[field]),
                           linewidth=4)
            pylab.ylabel(field)
    pylab.tight_layout()
    pylab.show()

if __name__ == '__main__':

    main()
