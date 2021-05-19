package Factory.service;

import Factory.dao.ClientDAO;
import Factory.model.*;
import java.util.List;

public class ClientService
{
    private static ClientDAO clientDao;

    public ClientService()
    {
        clientDao = new ClientDAO();
    }

    public void persist(Client entity) {
        clientDao.openCurrentSessionwithTransaction();
        clientDao.persist(entity);
        clientDao.closeCurrentSessionwithTransaction();
    }

    public void update(Client entity) {
        clientDao.openCurrentSessionwithTransaction();
        clientDao.update(entity);
        clientDao.closeCurrentSessionwithTransaction();
    }

    public Client findById(int id) {
        clientDao.openCurrentSession();
        Client contract = clientDao.findById(id);
        clientDao.closeCurrentSession();
        return contract;
    }

    public void delete(int id) {
        clientDao.openCurrentSessionwithTransaction();
        Client contract = clientDao.findById(id);
        clientDao.delete(contract);
        clientDao.closeCurrentSessionwithTransaction();
    }

    public List<Client> findAll() {
        clientDao.openCurrentSession();
        List<Client> contracts = clientDao.findAll();
        clientDao.closeCurrentSession();
        return contracts;
    }

    public void deleteAll() {
        clientDao.openCurrentSessionwithTransaction();
        clientDao.deleteAll();
        clientDao.closeCurrentSessionwithTransaction();
    }

    public ClientDAO clientDao()
    {
        return clientDao;
    }
}
