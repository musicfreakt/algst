package Factory.dao;
import Factory.model.*;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import java.util.List;

public class ClientDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public ClientDAO() { }

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

    public void persist(Client entity) {
        getCurrentSession().save(entity);
    }

    public void update(Client entity) {
        getCurrentSession().update(entity);
    }

    public Client findById(int id)
    {
        return getCurrentSession().get(Client.class, id);
    }

    public void delete(Client entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Client> findAll() {
        return (List<Client>) getCurrentSession().createQuery("SELECT c FROM clients c").list();
    }

    public void deleteAll() {
        List<Client> entityList = findAll();
        for (Client entity : entityList)
        {
            delete(entity);
        }
    }

}

