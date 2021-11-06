# CS3101_LibraryManagementSystem
It's a basic Library Management System written in <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="20" height="20"/>.
<hr>

<h2>There will be two types of users profile :</h2>
<ul>
    <li><h4>Admin</h4></li>
    <li><h4>User</h4></li>
</ul>
<h2>The Program shall do the following functions :</h2>
<ul>
    <li><h4>By Admin</h4></li>
        <ul>
            <li>Add Books</li>
            <li>Delete Books</li>
            <li>Update Books' details</li>
        </ul>
    <li><h4>By User</h4></li>
    <ul>
        <li>View Available Books</li>
        <li>Search Books</li>
        <li>Borrow Books</li>
        <li>Return Books</li>
    </ul>
</ul>
<br>
The Admin can do all things which User do.

<hr>
<hr>
<h2># Problem Question</h2>
Library system:

Design an interactive library system in C programming language. It should have two modes of access (i) Admin and (ii) User. There should be unique login ids for each. The system should ask for the login id and check it against a list. The admin and user ids for a person should be different. A person without a valid id should not be allowed to access the library. An Admin should typically be the librarian (or others responsible for the library). Users can be students or faculty members.

The allowed operations for each of these modes are:
Admin: Addition, deletion and updation of book records
User: Acquiring and returning of books

Each book should have a unique id, number of copies available, title, author, publisher etc. Each issue of a book should be recorded as well. The number of days for which a book can be borrowed should be dependent on the availability (more number of days for the books with a higher number of copies available). Also, there should be an upper limit on the number of books one can acquire at the same time. Books with less than 3 copies should be issued only to faculty members.

The system should have an interactive interface. It should show the current status of the availability and assign books accordingly. It should also take back books. The library database should be updated accordingly. It should also give the aforementioned rights to the admin.

In addition, a search facility should be present for both these modes allowing free-text search in general and also for specific fields of a book.
<hr>
