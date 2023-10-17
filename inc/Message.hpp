#include "../inc/Channels.hpp"
#include "../inc/Client.hpp"
#include "../inc/CommonLibs.hpp"
#include "../inc/Definition.hpp"
#include <vector>

typedef struct s_chanInfos
{
	int isFounded;
	int name;
	int index;
	int protection;
	int mode;
} t_chanInfos;

class Message
{
  public:
	Message(std::vector<Channels> &channelList, std::map<int, Client *> &clientList);
	bool handleInput(Client *client, std::string input);
	~Message();

  private:
	Message();
	void capHandler(t_cmd *input, int clientSocket);
	void listHandler(int clientSocket, std::string nickname);
	void namesHandler(t_cmd *input, Client *client);
	void joinHandler(t_cmd *input, Client *client);
	void userHandler(t_cmd *input, Client *client);
	void messageHandler(t_cmd *input, Client *client);
	void nickHandler(t_cmd *input, Client *client);
	void modeHandler(t_cmd *input, Client *client);
	void whoHandler(t_cmd *input, Client *client);
	void topicHandler(t_cmd *input, Client *client);
	t_cmd *parseInput(std::string &input);
	void canalMessage(t_cmd *input, Client *client);
	void privateMessage(t_cmd *input, Client *client);

	int channelExist(std::string chanName, t_chanInfos &infos, std::string password);
	int channelExist(std::string chanName, int (&chanInfo)[3], std::string password);
	std::vector<Channels> &_channels;
	std::map<int, Client *> &_clients;
	std::vector<Client *> _invitedClients;
	// verifier comment initialiser &_channels et &_clients
	// avec le constructeur par defaut
	std::vector<Channels> emptyVector;
	std::map<int, Client *> emptyMap;
	Channels _nullChannel;
	// creer une map avec les strings d'erreur / success;
};
