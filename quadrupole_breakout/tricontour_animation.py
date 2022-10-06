# QUADRUPOLE BREAKOUT

lims = 1000
col_map = 'jet'

def main_rho(fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed
    

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
        
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        
        levels = np.linspace(-1, 2, 25)

        with h5py.File(fname,'r') as f:
            p = np.array(f['hydrodynamic']['pressure'])
            rho = np.array(f['hydrodynamic']['density'])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            T = p/rho
            bs = rho**2*np.sqrt(T)
            
            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.99) & (R <= 1.01))]
            Ry = y[np.where((R >= 0.99) & (R <= 1.01))]
            
            # pylab.plot(Rx, Ry, 'r.')
            # pylab.plot(-Rx, Ry, 'r.')
            
            pylab.tricontourf(x, y,
                           np.log10(rho),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            pylab.tricontourf(-x, y,
                           np.log10(rho),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            
            pylab.title('Density Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='log(Density)')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_rho.mp4',
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)

def main_p(fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed
    

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
        
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        
        levels = np.linspace(-1, 2, 25)

        with h5py.File(fname,'r') as f:
            p = np.array(f['hydrodynamic']['pressure'])
            rho = np.array(f['hydrodynamic']['density'])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            T = p/rho
            bs = rho**2*np.sqrt(T)
            
            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.99) & (R <= 1.01))]
            Ry = y[np.where((R >= 0.99) & (R <= 1.01))]
            
            # pylab.plot(Rx, Ry, 'r.')
            # pylab.plot(-Rx, Ry, 'r.')
            
            pylab.tricontourf(x, y,
                           np.log10(p),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            pylab.tricontourf(-x, y,
                           np.log10(p),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            
            pylab.title('Pressure Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='log(Pressure)')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_p.mp4',
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)

            
def main_vel(fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed
    

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
        
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        
        levels = np.linspace(-1, 2, 25)

        with h5py.File(fname,'r') as f:
            p = np.array(f['hydrodynamic']['pressure'])
            rho = np.array(f['hydrodynamic']['density'])
            v_x = np.array(f['hydrodynamic']['x_velocity'])
            v_y = np.array(f['hydrodynamic']['y_velocity'])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            r = np.sqrt(x**2 + y**2)
            v_r = x/r*v_x + y/r*v_y
            
            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.99) & (R <= 1.01))]
            Ry = y[np.where((R >= 0.99) & (R <= 1.01))]
            
            # pylab.plot(Rx, Ry, 'r.')
            # pylab.plot(-Rx, Ry, 'r.')
            
            pylab.tricontourf(x, y,
                           v_r,
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            pylab.tricontourf(-x, y,
                           v_r,
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            
            pylab.title('Velocity Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='Velocity')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_vel.mp4',
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)
    
def main_c(fldr):
    
    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
        
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'
        
        levels = np.linspace(-10, 70, 25)

        with h5py.File(fname,'r') as f:
            p = np.array(f['hydrodynamic']['pressure'])
            rho = np.array(f['hydrodynamic']['density'])
            v_x = np.array(f['hydrodynamic']['x_velocity'])
            v_y = np.array(f['hydrodynamic']['y_velocity'])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            
            gamma = 4/3
            c_s = np.sqrt(gamma*p/rho)
            # r = np.sqrt(x**2 + y**2)
            # v_r = x/r*v_x + y/r*v_y
            
            T = p/rho
            bs = rho**2*np.sqrt(T)
            
            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.99) & (R <= 1.01))]
            Ry = y[np.where((R >= 0.99) & (R <= 1.01))]
            
            # pylab.plot(Rx, Ry, 'b.')
            # pylab.plot(-Rx, Ry, 'b.')
            
            pylab.tricontourf(x, y,
                              c_s, 140)#,
                              #levels=levels, extend = 'min')
            #pylab.clim((-70,70))
            pylab.tricontourf(-x, y,
                              c_s, 140)#,
                              #levels=levels, extend = 'min')
            
            pylab.title('Sound Speed Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='Sound Speed')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_soundspeed.mp4',
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)
            
            
def main_s(fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed
    

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
        
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'PuBu'
        
        levels = np.linspace(-1, 2, 25)

        with h5py.File(fname,'r') as f:
            p = np.array(f['hydrodynamic']['pressure'])
            rho = np.array(f['hydrodynamic']['density'])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            
            idx1 = np.where(f['stickers']['40'])
            idx2 = np.where(f['stickers']['41'])
            idx3 = np.where(f['stickers']['42'])
            idx4 = np.where(f['stickers']['43'])
            pylab.plot(x[idx1],y[idx1], 'w.')
            pylab.plot(x[idx2],y[idx2], 'w.')
            pylab.plot(x[idx3],y[idx3], 'w.')
            pylab.plot(x[idx4],y[idx4], 'w.')
            
            T = p/rho
            bs = rho**2*np.sqrt(T)
            gamma = 4./3.
            
            s = p/rho**gamma
            
            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.99) & (R <= 1.01))]
            Ry = y[np.where((R >= 0.99) & (R <= 1.01))]
            
            
            
            # pylab.plot(Rx, Ry, 'r.')
            # pylab.plot(-Rx, Ry, 'r.')
            
            pylab.tricontourf(x, y,
                           np.log10(s),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            pylab.tricontourf(-x, y,
                           np.log10(s),
                           140)#, levels=levels,
                             # extend='min')
            # pylab.clim((-9,5))
            
            pylab.title('Entropy Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='log(Entropy)')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_s.mp4',
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)
 

 
def main_tracer(t_name, fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))
    
    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'

        levels = np.linspace(0, 1, 50)

        with h5py.File(fname,'r') as f:
            t = np.array(f['tracers'][t_name])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])

            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.98) & (R <= 1.02))]
            Ry = y[np.where((R >= 0.98) & (R <= 1.02))]

            pylab.plot(Rx, Ry, "w.", markersize=1)
            pylab.plot(-Rx, Ry, "w.", markersize=1)
            
            pylab.tricontourf(x, y,
                              t, 140)
            pylab.tricontourf(-x, y,
                              t, 140)

            pylab.title('Tracer Concentration Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label="Tracer Concentration")
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_tracers_{:s}.mp4'.format(t_name),
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)            
 

 
def main_tracer_depth(t_name, fldr):

    import glob
    import re
    import numpy as np
    import h5py
    from tqdm import tqdm
    import imageio
    from joblib import Parallel, delayed

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))

    def snapshot2image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'
        import pylab    
        matplotlib.pyplot.rcParams['image.cmap'] = col_map#'RdBu_r'

        levels = np.linspace(0, 1, 50)

        with h5py.File(fname,'r') as f:
            t = np.array(f['tracers'][t_name])
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])

            R = np.sqrt(x**2 + y**2)
            Rx = x[np.where((R >= 0.98) & (R <= 1.02))]
            Ry = y[np.where((R >= 0.98) & (R <= 1.02))]

            pylab.plot(Rx, Ry, "w.", markersize=1)
            pylab.plot(-Rx, Ry, "w.", markersize=1)
            
            pylab.tricontourf(x, y,
                              t, 140,
                              levels=levels, extend = 'min')
            pylab.tricontourf(-x, y,
                              t, 140,
                              levels=levels, extend = 'min')

            pylab.title('Tracer Concentration Distribution \n t = %1.4f' % np.array(f['time'])[0])
            pylab.colorbar(label='Tracer Concentration')
            pylab.axis('scaled')
            pylab.xlim((-lims,lims))
            pylab.ylim((-lims,lims))
            pylab.xlabel('x')
            pylab.ylabel('y')
            pylab.savefig(fname.replace('.h5','.png'))
            pylab.close()

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
    #[snapshot2image(fname) for fname in tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(snapshot2image)(fname) for fname in tqdm(ss_files))

    with imageio.get_writer('contour_tracers_{:s}.mp4'.format(t_name),
                            mode='I') as writer:
                            #,duration=0.1) as writer:
        for fname in tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)
            

            
def main_verts(fldr):                                                                                                                                                                         
    import glob
    import re
    import numpy as np
    import h5py
    import tqdm
    import imageio
    from joblib import Parallel, delayed

    def extract_snapshot_number(fpath):

        return int(re.findall(r'_(\d+)',fpath)[-1])
        #return int(re.search(r'_(\d+)\.',fpath).group(1))

    def create_image(fname):

        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot
        matplotlib.pyplot.rcParams['image.cmap'] = 'jet'
        from matplotlib.patches import Polygon
        from matplotlib.collections import PatchCollection
        import pylab

        with h5py.File(fname,'r') as f:
            x = np.array(f['geometry']['x_coordinate'])
            y = np.array(f['geometry']['y_coordinate'])
            vert_x_raw = np.array(f['geometry']['x_vertices'])
            vert_y_raw = np.array(f['geometry']['y_vertices'])
            vert_n_raw = np.array(f['geometry']['n_vertices'])
            vert_idx_list = np.concatenate(([0],np.cumsum(vert_n_raw))).astype(int)

            polygon_list = [Polygon(
                    np.vstack((vert_x_raw[low_idx:high_idx],
                                  vert_y_raw[low_idx:high_idx])).T)
                            for low_idx, high_idx
                            in zip(vert_idx_list[:-1], vert_idx_list[1:])]

            patch_collection = PatchCollection(polygon_list, linestyle='-')
            #z_list = np.clip(np.log10(f['hydrodynamic']['density']), -20, 5)
            z_list = np.log10(f['hydrodynamic']['density']) 
            patch_collection.set_array(z_list)
            #patch_collection.set_array(np.log10(f['hydrodynamic']['density']))
            #patch_collection.set_array(f['hydrodynamic']['density'])
            fig, ax = pylab.subplots()
            ax.add_collection(patch_collection)
            pylab.plot(x,y, 'w.', markersize=1)
            pylab.suptitle('t = %.4f' % np.array(f['time'])[0])
            pylab.axis('scaled')
            pylab.xlim((0,lims))
            pylab.ylim((-lims,lims))
            pylab.savefig(fname.replace('.h5','.png'))

            pylab.close()
            pylab.close(fig)

    ss_files = sorted(glob.glob('{}/snapshot_*.h5'.format(fldr)),
                      key=extract_snapshot_number)
#    [create_image(fname) for fname in tqdm.tqdm(ss_files)]
    Parallel(n_jobs=-1)(delayed(create_image)(fname) for fname in tqdm.tqdm(ss_files))

    with imageio.get_writer('vertices.mp4',
                            mode='I') as writer:
        for fname in tqdm.tqdm(ss_files):
            image = imageio.imread(fname.replace('.h5','.png'))
            writer.append_data(image)           


            
if __name__ == '__main__':

    main_rho()

    
   