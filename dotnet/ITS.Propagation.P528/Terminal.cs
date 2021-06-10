using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Intermediate values for terminal geometry
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct Terminal
        {
            /// <summary>
            /// Real terminal height, in km
            /// </summary>
            public double h_r__km;

            /// <summary>
            /// Effective terminal height, in km
            /// </summary>
            public double h_e__km;

            /// <summary>
            /// Internal terminal param.  See Recommendation text
            /// </summary>
            public double delta_h__km;

            /// <summary>
            /// Ray traced horizon distance, in km
            /// </summary>
            public double d_r__km;

            /// <summary>
            /// Total ray path length to horizon, in km
            /// </summary>
            public double a__km;

            /// <summary>
            /// Central angle between the terminal and its smooth earth horizon
            /// </summary>
            public double phi__rad;

            /// <summary>
            /// Incident angle of the grazing ray at the terminal
            /// </summary>
            public double theta__rad;

            /// <summary>
            /// Median atmospheric absorption loss, in dB
            /// </summary>
            public double A_a__db;
        };
    }
}
