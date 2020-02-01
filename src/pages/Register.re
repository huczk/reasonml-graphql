open ReactUtils;

[@react.component]
let make = () => {
  <div className="auth-page">
    <div className="container page">
      <div className="row">
        <div className="col-md-6 offset-md-3 col-xs-12">
          <h1 className="text-xs-center"> "Sign up"->s </h1>
          <p className="text-xs-center">
            <Link href="#/login"> "Have an account?"->s </Link>
          </p>
          <RegisterForm />
        </div>
      </div>
    </div>
  </div>;
};