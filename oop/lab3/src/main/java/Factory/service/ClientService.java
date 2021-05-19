package Factory.service;

import Factory.dao.ClientDAO;
import Factory.model.*;
import java.util.List;

public class ClientService
{
    private static ClientDAO employeeDao;

    public ClientService()
    {
        employeeDao = new ClientDAO();
    }

    public void persist(Client entity) {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.persist(entity);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public void update(Client entity) {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.update(entity);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public Client findById(int id) {
        employeeDao.openCurrentSession();
        Client manager = employeeDao.findById(id);
        employeeDao.closeCurrentSession();
        return manager;
    }

    public void delete(int id) {
        employeeDao.openCurrentSessionwithTransaction();
        Client manager = employeeDao.findById(id);
        employeeDao.delete(manager);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public List<Client> findAll() {
        employeeDao.openCurrentSession();
        List<Client> managers = employeeDao.findAll();
        employeeDao.closeCurrentSession();
        return managers;
    }

    public void deleteAll() {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.deleteAll();
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public ClientDAO employeeDao()
    {
        return employeeDao;
    }
}
