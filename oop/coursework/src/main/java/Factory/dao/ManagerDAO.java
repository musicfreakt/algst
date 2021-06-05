package Factory.dao;
import Factory.model.*;

import Factory.util.HibernateSessionFactoryUtil;
import org.hibernate.Session;
import org.hibernate.Transaction;
import java.util.List;

public class ManagerDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public ManagerDAO() { }

    public Session openCurrentSession()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
        return currentSession;
    }

    public Session openCurrentSessionwithTransaction()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
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

    public void persist(Manager entity) {
        getCurrentSession().save(entity);
    }

    public void update(Manager entity) {
        getCurrentSession().update(entity);
    }

    public Manager findById(int id)
    {
        return getCurrentSession().get(Manager.class, id);
    }

    public void delete(Manager entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Manager> findAll() {
        return (List<Manager>) getCurrentSession().createQuery("SELECT m FROM managers m").list();
    }

    public void deleteAll() {
        List<Manager> entityList = findAll();
        for (Manager entity : entityList) {
            delete(entity);
        }
    }

}

