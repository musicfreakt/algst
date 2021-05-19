package Factory.dao;
import Factory.model.*;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import java.util.List;

public class ContractDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public ContractDAO() { }

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

    public void persist(Contract entity) {
        getCurrentSession().save(entity);
    }

    public void update(Contract entity) {
        getCurrentSession().update(entity);
    }

    public Contract findById(int id)
    {
        return getCurrentSession().get(Contract.class, id);
    }

    public void delete(Contract entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Contract> findAll() {
        return (List<Contract>) getCurrentSession().createQuery("SELECT c FROM contracts c").list();
    }

    public void deleteAll() {
        List<Contract> entityList = findAll();
        for (Contract entity : entityList)
        {
            delete(entity);
        }
    }

}

