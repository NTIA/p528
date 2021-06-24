using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Intermediate values for propagation path
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct Path
        {
            /// <summary>
            /// Maximum line-of-sight distance, in km
            /// </summary>
            public double d_ML__km;

            /// <summary>
            /// Internal param.  See Recommendation text
            /// </summary>
            public double d_0__km;

            /// <summary>
            /// Distance where smooth earth diffraction is 0 dB, in km
            /// </summary>
            public double d_d__km;
        };
    }
}
