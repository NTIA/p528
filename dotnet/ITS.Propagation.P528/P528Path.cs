using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Propagation
{
    /// <summary>
    /// Intermediate values for propagation path
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct P528Path
    {
        /// <summary>
        /// Maximum line-of-sight distance, in km
        /// </summary>
        public double d_ML__km;

        /// <summary>
        /// Internal param.  See Recommendation text.
        /// </summary>
        public double d_0__km;

        /// <summary>
        /// Distance where smooth earth diffraction is 0 dB
        /// </summary>
        public double d_d__km;

        /// <summary>
        /// Effective earth radius, in km
        /// </summary>
        public double a_e__km;
    };
}
