package Factory.dao;
import Factory.model.*;

import Factory.util.HibernateSessionFactoryUtil;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.query.Query;

import java.util.List;

public class ClientDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public ClientDAO() { }

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

    @SuppressWarnings("unchecked")
    public List<Client> findByManagerId(int id)
    {
        Query<Client> q = getCurrentSession().createQuery("SELECT c FROM clients c JOIN c.client_contracts s WHERE s.manager.id = :id");
        q.setParameter("id", id);
        return q.list();
    }

    public void deleteAll() {
        List<Client> entityList = findAll();
        for (Client entity : entityList)
        {
            delete(entity);
        }
    }

}

