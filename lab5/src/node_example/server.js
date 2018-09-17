var grpc = require('grpc');
var booksProto = grpc.load('books.proto');
var server = new grpc.Server();
var books = [];

server.addService(booksProto.books.BookService.service, {
	list: function(call, callback) {
		callback(null, books);
	},
	insert: function(call, callback) {
		var book = call.request;
		books.push(book);
		callback(null, {});
	},
	get: function(call, callback) {
		for (var i = 0; i < books.length; i++)
			if (books[i].id == call.request.id)
				return callback(null, books[i]);
			callback({
				code: grpc.status.NOT_FOUND,
				details: 'Not found'
			});
	},
	delete: function(call, callback) {
		for (var i = 0; i < books.length; i++) {
			if (books[i].id == call.request.id) {
				books.splice(i, 1);
				return callback(null, {});
			}
		}
		callback({
			code: grpc.status.NOT_FOUND,
			details: 'Not found'
		});
	}
});

server.bind('0.0.0.0:50051', grpc.ServerCredentials.createInsecure());
console.log('Server running at http://0.0.0.0:50051');
server.start();


