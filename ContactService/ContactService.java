package ContactService;
import java.util.Map;
import java.util.HashMap;

public class ContactService {
    private Map<String, Contact> contacts;

    public ContactService() {
        this.contacts = new HashMap<>();
    }

    public void addContact(String id, String firstName, String lastName, String phoneNumber, String address) {
        Contact contact = new Contact(id, firstName, lastName, phoneNumber, address);
        contacts.put(id, contact);
    }

    public void deleteContact(String id) {
        contacts.remove(id);
    }

    public void updateContactField(String id, String field, String value) {
        Contact contact = contacts.get(id);
        if (contact == null) {
            System.out.println("Contact not found.");
            return;
        }

        switch (field) {
            case "firstName":
                contact.setFirstName(value);
                break;
            case "lastName":
                contact.setLastName(value);
                break;
            case "number":
                contact.setPhone(value);
                break;
            case "address":
                contact.setAddress(value);
                break;
            default:
                System.out.println("Invalid field.");
        }
    }
    public Contact getContact(String id) {
        return contacts.get(id);
    }
}