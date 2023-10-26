using System;
using System.Collections.Generic;

namespace GrammarLearning
{
    internal class Library
    {
        private List<book> books;

        public delegate void BookAddedEventHandler(book item);

        // 事件声明
        public event BookAddedEventHandler BookAdded;

      
        public Library()
        {
            books = new List<book>();
        }

        // 添加一本书到图书馆
        public void AddBook(book newBook)
        {
            books.Add(newBook);
            Console.WriteLine($"Book '{newBook.Title}' by {newBook.Author} added to the library.");

            BookAdded?.Invoke(newBook);
        }

        // 从图书馆中移除一本书
        public void RemoveBook(book bookToRemove)
        {
            if (books.Contains(bookToRemove))
            {
                books.Remove(bookToRemove);
                Console.WriteLine($"Book '{bookToRemove.Title} ' by  {bookToRemove.Author} removed from the library.");
            }
            else
            {
                Console.WriteLine($"Book '{bookToRemove.Title} ' by  {bookToRemove.Author} is not in the library.");
            }
        }

        // 列出图书馆中的所有书籍
        public void ListBooks()
        {
            Console.WriteLine("Books in the library:");
            foreach (var b in books)
            {
                Console.WriteLine($"- {b.Title} by {b.Author}");
            }
        }

        // 查找长书（大于300页）
        public void ListLongBooks()
        {
            Console.WriteLine("Long books in the library:");
            foreach (var b in books)
            {
                if (b.isLong())
                {
                    Console.WriteLine($"- {b.Title} by {b.Author}");
                }
            }
        }

        public void printIntem(IPrintable item)
        {
            item.Print();
        }
    }
}
