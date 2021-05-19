package Factory.dao;
import Factory.model.*;

import javax.persistence.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import java.util.List;

public class SpecialisationDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public SpecialisationDAO() { }

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

    public void persist(Specialisation entity) {
        getCurrentSession().save(entity);
    }

    public void update(Specialisation entity) {
        getCurrentSession().update(entity);
    }

    public Specialisation findById(int id)
    {
        return getCurrentSession().get(Specialisation.class, id);
    }

    public Specialisation findByName(String name)
    {
        Query query = getCurrentSession().createQuery("FROM specialisations WHERE name = :name");
        query.setParameter("name", name);
        return (Specialisation) query.getSingleResult();
    }

    public void delete(Specialisation entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Specialisation> findAll() {
        return (List<Specialisation>) getCurrentSession().createQuery("SELECT s FROM specialisations s").list();
    }

    public void deleteAll() {
        List<Specialisation> entityList = findAll();
        for (Specialisation entity : entityList) {
            delete(entity);
        }
    }

}

