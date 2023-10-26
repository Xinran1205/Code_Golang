using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GrammarLearning
{
    internal class eBook : book
    {
        private double fileSize;  // 文件大小 (例如以MB为单位)
        private string fileFormat;  // 文件格式 (例如 "pdf", "epub" 等)

        // 默认构造函数
        public eBook() : base()
        {
            fileSize = 0.0;
            fileFormat = string.Empty;
        }

        // 参数化构造函数
        public eBook(string title, string author, int pages, double fileSize, string fileFormat)
            : base(title, author, pages)
        {
            this.fileSize = fileSize;
            this.fileFormat = fileFormat;
        }

        // 获取或设置fileSize
        public double FileSize
        {
            get { return fileSize; }
            set { fileSize = value; }
        }

        // 获取或设置fileFormat
        public string FileFormat
        {
            get { return fileFormat; }
            set { fileFormat = value; }
        }

        // EBook类特有的方法
        public void Download()
        {
            Console.WriteLine($"Downloading {Title} in {fileFormat} format...");
        }
    }
}
