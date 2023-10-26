using System;
using GrammarLearning;

class Program
{
    static void Main()
    {
        Library library = new Library();

        // 事件订阅
        library.BookAdded += OnBookAdded;

        book newBook = new book("C# Programming", "John Doe", 320);
        library.AddBook(newBook);
    }

    // 事件处理程序
    static void OnBookAdded(book addedBook)
    {
        Console.WriteLine($"Event triggered: '{addedBook.Title}' by {addedBook.Author} has been added.");
    }
}