package Factory.dao;
import Factory.model.*;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import java.util.List;

public class EmployeeDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public EmployeeDAO() { }

    public Session openCurrentSession()
    {
        currentSession = getSessionFactory().openSession();
        return currentSession;
    }

    public Session openCurrentSessionwithTransaction()
    {
        currentSession = getSessionFactory().openSession();
        currentTransaction = currentSession.beginTransaction();
        return currentSession;
    }

    public void closeCurrentSession()
    {
        currentSession.close();
    }

    public void closeCurrentSessionwithTransaction()
    {
        currentTransaction.commit();
        currentSession.close();
    }

    private static SessionFactory getSessionFactory()
    {
        Configuration configuration = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Person.class)
                .addAnnotatedClass(Manager.class)
                .addAnnotatedClass(Specialisation.class)
                .addAnnotatedClass(Employee.class)
                .addAnnotatedClass(Client.class)
                .addAnnotatedClass(Contract.class);
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder()
                .applySettings(configuration.getProperties());
        return configuration.buildSessionFactory(builder.build());
    }

    public Session getCurrentSession() {
        return currentSession;
    }

    public void setCurrentSession(Session currentSession) {
        this.currentSession = currentSession;
    }

    public Transaction getCurrentTransaction() {
        return currentTransaction;
    }

    public void setCurrentTransaction(Transaction currentTransaction) {
        this.currentTransaction = currentTransaction;
    }

    public void persist(Employee entity) {
        getCurrentSession().save(entity);
    }

    public void update(Employee entity) {
        getCurrentSession().update(entity);
    }

    public Employee findById(int id)
    {
        return getCurrentSession().get(Employee.class, id);
    }

    public void delete(Employee entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Employee> findAll() {
        return (List<Employee>) getCurrentSession().createQuery("SELECT e FROM employees e").list();
    }

    public void deleteAll() {
        List<Employee> entityList = findAll();
        for (Employee entity : entityList)
        {
            delete(entity);
        }
    }

}

