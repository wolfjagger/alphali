#pragma once

#include "publisher.h"



namespace alphali {

	class collaborator {

	private:

		publisher pub;
		subscriber sub;

	public:

		collaborator() = default;
		collaborator(const publisher& init_pub, const subscriber& init_sub);
		collaborator(const publisher& init_pub, subscriber&& init_sub);
		collaborator(publisher&& init_pub, const subscriber& init_sub);
		collaborator(publisher&& init_pub, subscriber&& init_sub);

		void set_publisher(const publisher& new_pub);
		void set_publisher(publisher&& new_pub);
		void set_subscriber(const subscriber& new_sub);
		void set_subscriber(subscriber&& new_sub);



		void publish();

		void subscribe(collaborator& other, std::function<void()> fcn_update);
		void unsubscribe(collaborator& other);
		void subscribe(publisher& other, std::function<void()> fcn_update);
		void unsubscribe(publisher& other);



		friend void subscriber::subscribe(
			collaborator&, std::function<void()>);
		friend void subscriber::unsubscribe(collaborator&);

	};



	void connect(
		collaborator& collab1, collaborator& collab2,
		std::function<void()> responder1, std::function<void()> responder2);

	void disconnect(collaborator& collab1, collaborator& collab2);

}
