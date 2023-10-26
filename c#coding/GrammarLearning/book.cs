using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GrammarLearning
{
    internal class book : IPrintable
    {
        private string title;
        private string author;
        private int pages;
        private static int bookCount = 0;

        public string Title
        {
            get { return title; }
            set { title = value; }
        }

        public string Author
        {
            get { return author; }
            set { author = value; }
        }

        public int Pages
        {
            get { return pages; }
            set { pages = value; }
        }


        public book()
        {
            Console.WriteLine("Creating book...");
            bookCount++;
        }

        public static int getBookCount()
        {
            return bookCount;
        }

        public book(string aTitle, string aAuthor, int aPages)
        {
            title = aTitle;
            author = aAuthor;
            pages = aPages;
            bookCount++;
        }

        public bool isLong()
        {
            if (pages >= 300)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public void Print()
        {
            Console.WriteLine("Printing book: " + title);
        }
    }
}
