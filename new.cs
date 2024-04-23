using System;

class EducationInstitution
{
    private string name;
    private int enrolledStudents;

    public EducationInstitution(string name)
    {
        this.name = name;
        this.enrolledStudents = 0;
    }

    public void DismissStudent(int count)
    {
        enrolledStudents -= count;
    }

    public void PrintInfo()
    {
        Console.WriteLine($"Учебное заведение: {name}, Зачислено учащихся: {enrolledStudents}");
    }
}

class School : EducationInstitution
{
    private int numberOfSpecializedClasses;

    public School(string name, int numberOfSpecializedClasses) : base(name)
    {
        this.numberOfSpecializedClasses = numberOfSpecializedClasses;
    }

    public void EnrollInSpecializedClass(int count)
    {
        base.DismissStudent(count);
        Console.WriteLine($"Зачислено учащихся в профильный класс: {count}");
    }

    public void PrintInfo()
    {
        base.PrintInfo();
        Console.WriteLine($"Количество профильных классов: {numberOfSpecializedClasses}");
    }
}

class University : EducationInstitution
{
    private int numberOfFaculties;

    public University(string name, int numberOfFaculties) : base(name)
    {
        this.numberOfFaculties = numberOfFaculties;
    }

    public void EnrollInFaculty(int count)
    {
        base.DismissStudent(count);
        Console.WriteLine($"Зачислено абитуриентов на факультет: {count}");
    }

    public void PrintInfo()
    {
        base.PrintInfo();
        Console.WriteLine($"Количество факультетов: {numberOfFaculties}");
    }
}

class Program
{
    static void Main()
    {
        School school = new School("Школа какая-то там", 25);
        University university = new University("Университет какой-то там", 45);

        Random random = new Random();
        int randomStudentsInClass = random.Next(1, 10);
        int randomApplicantsInFaculty = random.Next(1, 20);

        school.EnrollInSpecializedClass(randomStudentsInClass);
        university.EnrollInFaculty(randomApplicantsInFaculty);

        Console.WriteLine("Информация о школе:");
        school.PrintInfo();

        Console.WriteLine("\nИнформация об университете:");
        university.PrintInfo();
    }
}