namespace SamplesCSharp
{
    internal class Program
    {
        static void DumpFile(string sFilename, string sUID, int serNo = 1, int imgNo = 1)
        {
            QMTDicomFile message = new QMTDicomFile();
            message.Load(sFilename);
            Console.WriteLine("SOP Instance UID: " + message.SOPInstanceUID);
            Console.WriteLine("SOP Class UID   : " + message.SOPClassUID);
            Console.WriteLine("Number of Frames: " + message.NumberOfFrames);
            foreach (QMTDicomTag tag in message)
            {
                if (tag.PrivateCreator.Length > 0)
                    Console.WriteLine(string.Format("{0:x4}:{1:x4} '{2}' [{3}] ", tag.Group, tag.Element, tag.VR, tag.PrivateCreator));
                else
                    Console.WriteLine(string.Format("{0:x4}:{1:x4} '{2}'", tag.Group, tag.Element, tag.VR));
            }
            string sOutfile = System.IO.Path.GetDirectoryName(sFilename) + "\\" + System.IO.Path.GetFileNameWithoutExtension(sFilename) + ".dcm";
            message.SetUIDs(sUID, sUID + "." + serNo.ToString(), sUID + "." + serNo.ToString() + "." + imgNo.ToString());
            message.Save(sOutfile);//message.SaveXML(@"d:\buf\temp\00010017.xml");
        }
        static void Main(string[] args)
        {
            QMTDicomFile message = new QMTDicomFile();
            Console.WriteLine("V" + message.Version);
            string sStudy = QMTDicomFile.CreateInstanceUID(3710);
            DumpFile(@"d:\buf\dlr\SHM4FCAS\6 mm\6 mm 32J\US-6 mm Impact 32J_D.dcmt", sStudy, 1);
            DumpFile(@"d:\buf\dlr\SHM4FCAS\6 mm\6 mm 32J\US-6 mm Impact 32J_C.dcmt", sStudy, 2);
            DumpFile(@"d:\buf\dlr\SHM4FCAS\6 mm\6 mm 32J\US-6 mm Impact 32J_CC.dcmt", sStudy, 3);
            //message.Load(@"d:\buf\dlr\SHM4FCAS\6 mm\6 mm 32J\US-6 mm Impact 32J_D.dcmt", true);
            //Console.WriteLine("SOP Instance UID: " + message.SOPInstanceUID);
            //Console.WriteLine("SOP Class UID   : " + message.SOPClassUID);
        }
    }
}
