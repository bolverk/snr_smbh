#include "my_main_loop.hpp"
#include "source/newtonian/test_2d/main_loop_2d.hpp"
#include "source/newtonian/test_2d/consecutive_snapshots.hpp"
#include "write_conserved.hpp"
#include "write_cycle.hpp"
#include "source/newtonian/test_2d/multiple_diagnostics.hpp"
#include "source/tessellation/shape_2d.hpp"
#include "supernova.hpp"
#include "source/misc/vector_initialiser.hpp"

using namespace simulation2d;

void my_main_loop(hdsim& sim, const Constants& c)
{
  const double tf = 1e2*c.year;
  SafeTimeTermination term_cond_raw(tf,1e6);
  MultipleDiagnostics diag
    (VectorInitialiser<DiagnosticFunction*>()
     (new ConsecutiveSnapshots(new ConstantTimeInterval(tf/10),
			       new Rubric("snapshot_",".h5")))
     (new WriteTime("time.txt"))
     (new WriteCycle("cycle.txt"))());
  const Circle hot_spot(Vector2D(0,-c.offset),
			c.supernova_radius);
  Supernova manip(hot_spot,
		  c.supernova_mass,
		  c.supernova_energy,
		  2e4*c.year);
  main_loop(sim, 
	    term_cond_raw,
	    &hdsim::TimeAdvance, 
	    &diag,
	    &manip);
}
