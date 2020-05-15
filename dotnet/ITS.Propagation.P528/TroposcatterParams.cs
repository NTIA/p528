using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-4
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Intermediate values for troposcatter calculations
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct TroposcatterParams
        {
            /// <summary>
            /// Scattering distance 
            /// </summary>
            public double d_s__km;

            /// <summary>
            /// Half the scattering distance
            /// </summary>
            public double d_z__km;

            /// <summary>
            /// Height of the common volume cross-over point
            /// </summary>
            public double h_v__km;

            /// <summary>
            /// Scattering angle
            /// </summary>
            public double theta_s;

            /// <summary>
            /// Cross-over angle
            /// </summary>
            public double theta_A;

            /// <summary>
            /// Troposcattter Loss
            /// </summary>
            public double A_s__db;

            /// <summary>
            /// Troposcatter Loss of Previous Test Point
            /// </summary>
            public double A_s_prev__db;

            /// <summary>
            /// Troposcatter Line Slope
            /// </summary>
            public double M_s;
        };
    }
}
