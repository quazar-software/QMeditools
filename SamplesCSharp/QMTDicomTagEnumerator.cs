using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SamplesCSharp
{
    public class QMTDicomTagEnumerator : IEnumerator<QMTDicomTag>
    {
        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern void QMTReleaseEnumerator(IntPtr h);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern IntPtr QMTGetNextObject(IntPtr hEnum, [In, Out] QMT_TAGINFO Info);

        // Declares a class member for each structure element.
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public class QMT_TAGINFO
        {
            public UInt32 nGroup = 0;
            public UInt32 nElement = 0;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szPrivateCreator = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2)]
            public string cVR = null;
            public UInt32 nNumberOfValues = 0;
            public UInt64 nDataLength = 0;
            public UInt64 nOffset = 0;
        }
        public QMTDicomTagEnumerator(IntPtr hEnum)
        {
            _hEnum = hEnum;
        }
        private IntPtr _hEnum;
        private bool disposedValue;

        private QMTDicomTag _tag = null;
        public QMTDicomTag Current { get { return _tag; } }

        object IEnumerator.Current => throw new NotImplementedException();

        public bool MoveNext()
        {
            if (_hEnum == IntPtr.Zero) return false;
            IntPtr hTag = QMTGetNextObject(_hEnum, null);
            _tag = new QMTDicomTag(hTag);
            return (hTag != IntPtr.Zero);
        }

        public void Reset()
        {
            throw new NotImplementedException();
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }
                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                // TODO: set large fields to null
                if(_hEnum != IntPtr.Zero) QMTReleaseEnumerator(_hEnum);
                _hEnum = IntPtr.Zero;
                disposedValue = true;
            }
        }

        // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        ~QMTDicomTagEnumerator()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
